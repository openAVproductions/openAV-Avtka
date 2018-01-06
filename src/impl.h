/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>

#include "avtka.h"

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#define AVTKA_ERROR(avtka, fmt, ...)					\
	do { fprintf(stderr, "[\033[1;31m%s +%d\033[0m] " fmt,		\
		__func__, __LINE__, __VA_ARGS__);			\
	} while (0)
#define AVTKA_WARN(avtka, fmt, ...)					\
	do { if (avtka->opts.debug_level >= AVTKA_DEBUG_WARN)		\
	fprintf(stderr, "[\033[1;33m%s +%d\033[0m] " fmt,		\
		__func__, __LINE__, __VA_ARGS__);			\
	} while (0)
#define AVTKA_INFO(avtka, fmt, ...)					\
	do { if (avtka->opts.debug_level >= AVTKA_DEBUG_INFO)		\
	fprintf(stderr, "[\033[1;32m%s +%d\033[0m] " fmt,		\
		__func__, __LINE__, __VA_ARGS__);			\
	} while (0)

#define AVTKA_MAX_ITEMS 1024
#define AVTKA_MAX_DRAW 8
#define AVTKA_MAX_SCREENS 2

struct avtka_item_t {
	/* public state of widget */
	struct avtka_item_opts_t opts;
	/* private state of widget */
	float value;
	uint32_t col;
};

/* draw function callback */
typedef void (*avtka_draw)(struct avtka_t *, struct avtka_item_t *,
			   void *cairo);

void draw_button(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_dial(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_jog_wheel(struct avtka_t *a, struct avtka_item_t *item, void* c);
void draw_led_strip(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_slider(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_filter(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_7seg(struct avtka_t *a, struct avtka_item_t *item, void* cairo);

int32_t avtka_interact_press(struct avtka_t *a, uint32_t item,
			     uint32_t x, uint32_t y);
int32_t avtka_interact_release(struct avtka_t *a, uint32_t item,
			       uint32_t x, uint32_t y);
int32_t avtka_interact_motion(struct avtka_t *a, uint32_t item,
			      int32_t x, int32_t y);

struct avtka_t {
	struct avtka_opts_t opts;

	uint8_t quit;
	uint8_t entered;
	float rescale;
	float drag_sensitivity;


	/* implementation details */
	void* pugl;

	/* all items in the UI */
	uint32_t item_count;
	struct avtka_item_t items[AVTKA_MAX_ITEMS];

	/* virtual screen surfaces */
	uint16_t screen_count;
	void *screen_surfaces[AVTKA_MAX_SCREENS];
	struct avtka_screen_opts_t screen_opts[AVTKA_MAX_SCREENS];

	/* array of draw routines */
	avtka_draw draw[AVTKA_MAX_DRAW];

	/* special cased widgets */
	uint32_t clicked_item;
	int32_t clicked_x;
	int32_t clicked_y;

	/* damage history, see redraw_get_damaged_area() */
	uint32_t damage_x;
	uint32_t damage_y;
	uint32_t damage_w;
	uint32_t damage_h;
	uint16_t damage_rev;
	uint16_t rev;

	/* lookup table of cairo_pattern_t pointers for colours */
	uint16_t cols_used;
	void *cols[UINT8_MAX];
};


uint32_t avtka_item_contact(struct avtka_t *a, uint32_t x, uint32_t y);


/** Colour Set for lookup table
 * This idea is sneaky - it allows setting a colour, and the dark/light
 * variants are calculated based on some HSV magic. This allows toolkits
 * to not have to specify details - just primary and secondary colours.
 * Variations are pre-computed at load-time, and used via the Colours
 * defines as normal.
 */
enum AVTKA_COLOURS {
	AVTKA_COL_BG_D,
	AVTKA_COL_BG,
	AVTKA_COL_BG_L,
	AVTKA_COL_BG_T,

	AVTKA_COL_PRI1_D,
	AVTKA_COL_PRI1,
	AVTKA_COL_PRI1_L,
	AVTKA_COL_PRI1_T,
};

static inline void
item_damage(struct avtka_t *a, uint32_t item)
{
	uint32_t x = a->items[item].opts.x;
	uint32_t y = a->items[item].opts.y;
	uint32_t w = a->items[item].opts.w;
	uint32_t h = a->items[item].opts.h;
	if(x < a->damage_x)
		a->damage_x = x;
	if(y < a->damage_y)
		a->damage_y = y;
	if(w > a->damage_w)
		a->damage_w = w;
	if(h > a->damage_h)
		a->damage_h = h;
}

