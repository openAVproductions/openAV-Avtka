/* This file is part of AVTKA by OpenAV */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "avtka.h"

enum ITEMS {
	ITEM_DIAL,
	ITEM_SLIDER_V,
	ITEM_SLIDER_H,
	ITEM_BUTTON,
	ITEM_JOG_WHEEL,
	ITEM_FILTER,
	ITEM_7SEG,

	ITEM_COUNT,
};

struct demo_t {
	uint32_t items[ITEM_COUNT];
	uint8_t cols[2];
	uint8_t col_counter;
};

void event_cb(struct avtka_t *avtka, uint32_t item, float v, void *userdata)
{
	struct demo_t *demo = userdata;
	printf("event on item %d, value %f\n", item, v);

	avtka_item_value(avtka, item, v);

	avtka_item_value(avtka, demo->items[ITEM_7SEG], v);

	if(item == demo->items[ITEM_BUTTON] && v > 0.5) {
		demo->col_counter++;
		for(int i = 0; i < ITEM_COUNT; i++) {
			avtka_item_colour(avtka, demo->items[i],
					  demo->cols[demo->col_counter % 2]);
		}
	}

	if(item == demo->items[ITEM_FILTER] ||
	   item == demo->items[ITEM_SLIDER_V]) {
		avtka_item_value(avtka, demo->items[ITEM_SLIDER_V], v);
		avtka_item_value(avtka, demo->items[ITEM_FILTER], v);
	}
}

int main()
{
	struct demo_t demo;
	memset(&demo, 0, sizeof(struct demo_t));
	demo.cols[0] = 5;
	demo.cols[1] = 9;

	struct avtka_opts_t opts = {
		.w = 360,
		.h = 240,
		.event_callback = event_cb,
		.event_callback_userdata = &demo,
		// uncomment for fixed size
		//.flag_no_resize = 1,
	};
	struct avtka_t *a = avtka_create("AVTKA v0.1", &opts);
	if(!a) {
		printf("failed to create avtka instance, quitting\n");
		return 0;
	}

	struct avtka_item_opts_t item = {
		.name = "Dial 1",
		.x = 10, .y = 10, .w = 50, .h = 50,
		.draw = AVTKA_DRAW_DIAL,
		.interact = AVTKA_INTERACT_DRAG_V,
	};
	demo.items[ITEM_DIAL] = avtka_item_create(a, &item);

	item.x = 70;
	item.h = 150;
	item.draw = AVTKA_DRAW_SLIDER;
	item.interact = AVTKA_INTERACT_DRAG_V;
	snprintf(item.name, sizeof(item.name), "Slider 1");
	demo.items[ITEM_SLIDER_V] = avtka_item_create(a, &item);

	item.x = 130;
	item.w = 150;
	item.h =  30;
	item.draw = AVTKA_DRAW_SLIDER;
	item.interact = AVTKA_INTERACT_DRAG_H;
	snprintf(item.name, sizeof(item.name), "Slider (Horizontal)");
	demo.items[ITEM_SLIDER_H] = avtka_item_create(a, &item);

	item.y =  80;
	item.h =  30;
	item.w =  30;
	item.draw = AVTKA_DRAW_BUTTON;
	item.interact = AVTKA_INTERACT_CLICK;
	snprintf(item.name, sizeof(item.name), "BTN");
	demo.items[ITEM_BUTTON] = avtka_item_create(a, &item);

	item.x = 180;
	item.y = 100;
	item.w = 100;
	item.h = 100;
	item.draw = AVTKA_DRAW_JOG_WHEEL;
	item.interact = AVTKA_INTERACT_DRAG_DELTA_V;
	snprintf(item.name, sizeof(item.name), "Jog Wheel");
	demo.items[ITEM_JOG_WHEEL] = avtka_item_create(a, &item);

	item.x =  80;
	item.y = 160;
	item.w = 100;
	item.h =  80;
	item.draw = AVTKA_DRAW_FILTER;
	item.interact = AVTKA_INTERACT_DRAG_V;
	snprintf(item.name, sizeof(item.name), "Filter");
	demo.items[ITEM_FILTER] = avtka_item_create(a, &item);

	item.x = 20;
	item.y = 130;
	item.w = 42;
	item.h = 16;
	item.params[0] = 3;
	item.draw = AVTKA_DRAW_7_SEG;
	item.interact = AVTKA_INTERACT_DRAG_V;
	snprintf(item.name, sizeof(item.name), "7 Seg");
	demo.items[ITEM_7SEG] = avtka_item_create(a, &item);

	avtka_run(a);

	return 0;
}
