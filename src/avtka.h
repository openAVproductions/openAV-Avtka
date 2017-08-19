/* This file is part of AVTKA by OpenAV */

#ifndef AVTKA_H
#define AVTKA_H

#include <stdint.h>

#define ITEM_NAME_MAX 16

/* forward declarations */
struct avtka_t;
struct avtka_item_t;

struct avtka_opts_t {
	uint32_t flags;
	uint16_t w, h;
	uint8_t padding[63];
};

enum AVTKA_DRAW_IDS {
	AVTKA_DRAW_NONE   = 0,
	AVTKA_DRAW_DIAL   = 1,
	AVTKA_DRAW_SLIDER = 2,
};

struct avtka_item_opts_t {
	char name[ITEM_NAME_MAX];
	uint16_t x, y, w, h;
	uint8_t draw;
	uint8_t padding[23];
};

/* Instantiate a new window */
struct avtka_t *avtka_create(const char *window_name,
			     struct avtka_opts_t *opts);

/* Update and redraw the UI */
void avtka_iterate(struct avtka_t *a);
/* Run unit a quit command is issued by the user */
void avtka_run(struct avtka_t *a);

/* Quit and cleanup a ui */
int32_t avtka_destroy(struct avtka_t *a);

/* Set visibility of the item */
void avtka_item_visible_set(struct avtka_t *a,
			    uint32_t item,
			    uint32_t visible);

/* Create/destroy an item */
uint32_t avtka_item_create(struct avtka_t *a, struct avtka_item_opts_t *o);
int32_t avtka_item_destroy(struct avtka_t *a, uint32_t item_id);

#endif /* AVTKA_H */
