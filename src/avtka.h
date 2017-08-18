/* This file is part of AVTKA by OpenAV */

#ifndef AVTKA_H
#define AVTKA_H

#include <stdint.h>

/* forward declarations */
struct avtka_ui_t;
struct avtka_item_t;

struct avtka_ui_opts_t {
	uint32_t flags;
	uint8_t padding[63];
};

/* Instantiate a new window */
struct avtka_ui_t *avtka_ui_create(const char *window_name,
				   uint32_t width, uint32_t height,
				   struct avtka_ui_opts_t *opts);

/* Set visibility of the item */
void avtka_item_visible_set(struct avtka_item_t *item, uint32_t visible);

/* Quit and cleanup a ui */
int32_t avtka_ui_destroy(struct avtka_ui_t *ui);

/* Create a new item */
struct avtka_item_t *avtka_item_create();
int32_t avtka_item_destroy(struct avtka_item_t *);

/* Add a widget to a new container */
int32_t avtka_item_reparent(struct avtka_item_t *new_parent,
			    struct avtka_item_t *old_parent);


#endif /* AVTKA_H */
