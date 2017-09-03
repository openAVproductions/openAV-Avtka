/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>

#include "avtka.h"

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

void draw_dial(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_slider(struct avtka_t *a, struct avtka_item_t *item, void* cairo);
void draw_button(struct avtka_t *a, struct avtka_item_t *item, void* cairo);

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
};


uint32_t avtka_item_contact(struct avtka_t *a, uint32_t x, uint32_t y);
