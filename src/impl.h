/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>

#include "avtka.h"

#define AVTKA_MAX_ITEMS 1024

/* draw function callback */
typedef void (*avtka_draw)(struct avtka_t, struct avtka_item_t *);

struct avtka_item_t {
	avtka_item_draw draw;
};

struct avtka_t {
	uint32_t item_count;
	/* a ui pointer can be treated as an item */
	struct avtka_item_t items[AVTKA_MAX_ITEMS];

	/* implementation details */
	PuglView* pugl;
};
