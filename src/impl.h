/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>

#include "avtka.h"

#define AVTKA_MAX_ITEMS 1024
#define AVTKA_MAX_DRAW 8

/* draw function callback */
typedef void (*avtka_draw)(struct avtka_t, struct avtka_item_t *);

struct avtka_item_t {
	/* public state of widget */
	struct avtka_item_opts_t opts;
	/* private state of widget */
	uint8_t clicked;
};

struct avtka_t {
	uint8_t quit;
	uint8_t entered;

	/* implementation details */
	void* pugl;

	/* all items in the UI */
	uint32_t item_count;
	struct avtka_item_t items[AVTKA_MAX_ITEMS];

	/* array of draw routines */
	avtka_draw draw[AVTKA_MAX_DRAW];

};
