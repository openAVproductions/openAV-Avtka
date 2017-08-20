/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>

#include "avtka.h"

#define AVTKA_MAX_ITEMS 1024
#define AVTKA_MAX_DRAW 8

/* draw function callback */
typedef void (*avtka_draw)(struct avtka_t *, struct avtka_item_t *,
			   void *cairo);

void draw_dial(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_slider(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_button(struct avtka_t *a, struct avtka_item_t *item, void* cairo);

struct avtka_item_t {
	/* public state of widget */
	struct avtka_item_opts_t opts;
	/* private state of widget */
	uint8_t clicked;
};

struct avtka_t {
	uint8_t quit;
	uint8_t entered;

	struct avtka_opts_t opts;

	/* implementation details */
	void* pugl;

	/* all items in the UI */
	uint32_t item_count;
	struct avtka_item_t items[AVTKA_MAX_ITEMS];

	/* array of draw routines */
	avtka_draw draw[AVTKA_MAX_DRAW];

};
