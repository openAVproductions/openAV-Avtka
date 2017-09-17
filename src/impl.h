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

struct avtka_item_t {
	/* public state of widget */
	struct avtka_item_opts_t opts;
	/* private state of widget */
	float value;
};

/* draw function callback */
typedef void (*avtka_draw)(struct avtka_t *, struct avtka_item_t *,
			   void *cairo);

void draw_button(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_dial(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_jog_wheel(struct avtka_t *a, struct avtka_item_t *item, void* c);
void draw_led_strip(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_slider(struct avtka_t *a, struct avtka_item_t *item, void* cairo);

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

	/* array of draw routines */
	avtka_draw draw[AVTKA_MAX_DRAW];

	/* special cased widgets */
	uint32_t clicked_item;
	int32_t clicked_x;
	int32_t clicked_y;

	/* lookup table of cairo_pattern_t pointers for colours */
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
