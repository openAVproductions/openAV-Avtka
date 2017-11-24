/* This file is part of AVTKA by OpenAV */

#ifndef AVTKA_H
#define AVTKA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define ITEM_NAME_MAX 16

/* Errors only is default */
#define AVTKA_DEBUG_ERROR 0
#define AVTKA_DEBUG_WARN  1
#define AVTKA_DEBUG_INFO  2

/* forward declarations */
struct avtka_t;

typedef void (*avtka_event_cb)(struct avtka_t *avtka, uint32_t item,
			       float value, void *userdata);

struct avtka_opts_t {
	uint32_t flags;
	uint16_t w;
	uint16_t h;
	avtka_event_cb event_callback;
	void *event_callback_userdata;
	/* debug verbosity */
	uint8_t debug_level;

	uint8_t padding[39];
};

enum AVTKA_DRAW_IDS {
	AVTKA_DRAW_NONE   = 0,
	AVTKA_DRAW_BUTTON = 1,
	AVTKA_DRAW_DIAL   = 2,
	AVTKA_DRAW_SLIDER = 3,
	AVTKA_DRAW_JOG_WHEEL = 4,
	AVTKA_DRAW_LED_STRIP = 5,
	AVTKA_DRAW_FILTER = 6,
};
enum AVTKA_INTERACT_IDS {
	AVTKA_INTERACT_NONE   = 0,
	AVTKA_INTERACT_CLICK  = 1,
	AVTKA_INTERACT_TOGGLE = 2,
	AVTKA_INTERACT_DRAG_V = 3,
	AVTKA_INTERACT_DRAG_H = 4,
	AVTKA_INTERACT_DRAG_DELTA_V = 5,
	AVTKA_INTERACT_DRAG_DELTA_H = 6,
};

struct avtka_item_opts_t {
	uint16_t x, y, w, h;
	uint8_t draw;
	uint8_t interact;
	uint8_t colour;
	uint8_t params[4];
	/* bitset of options */
	uint8_t show_label   : 1;
	uint8_t flags_unused : 7;
	char name[ITEM_NAME_MAX];
};

/* Instantiate a new window */
struct avtka_t *avtka_create(const char *window_name,
			     struct avtka_opts_t *opts);

void avtka_redraw(struct avtka_t *a);
/* Update and redraw the UI */
void avtka_iterate(struct avtka_t *a);
/* Run unit a quit command is issued by the user */
void avtka_run(struct avtka_t *a);
/* Register a colour to be used by items */
uint8_t avtka_register_colour(struct avtka_t *avtka,
			      float r, float g, float b, float a);

/* Quit and cleanup a ui */
int32_t avtka_destroy(struct avtka_t *a);

struct avtka_screen_opts_t {
	uint16_t x, y, w, h;
	uint16_t px_x, px_y;
	/* single bit per pixel; on off only */
	uint32_t flags_1bit : 1;
	/* rgb capable screen */
	uint32_t flags_rgb  : 1;
};

/* allocate a new screen in the UI.
 * Returns 0 or a positive number as the screen-id on success.
 * Returns -1 on invalid screen_opts, or  failure to allocate memory
 */
int32_t
avtka_screen_create(struct avtka_t *a, struct avtka_screen_opts_t *opts);
uint8_t *avtka_screen_get_data_ptr(struct avtka_t *a, uint16_t screen_id);

/* Set visibility of the text label */
void avtka_item_label_show(struct avtka_t *a, uint32_t item, uint32_t show);
/* Set value of the item */
void avtka_item_value(struct avtka_t *a, uint32_t item, float value);
void avtka_item_colour32(struct avtka_t *a, uint32_t item, uint32_t col);
void avtka_item_value_inc(struct avtka_t *a, uint32_t item, float value);
/* Set the primary for the item */
void avtka_item_colour(struct avtka_t *a, uint32_t item, uint8_t col);

/* Create/destroy an item */
uint32_t avtka_item_create(struct avtka_t *a, struct avtka_item_opts_t *o);
int32_t avtka_item_destroy(struct avtka_t *a, uint32_t item_id);

#ifdef __cplusplus
}
#endif


#endif /* AVTKA_H */
