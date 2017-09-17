/* This file is part of AVTKA by OpenAV */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "avtka.h"

enum ITEMS {
	ITEM_DIAL,
	ITEM_SLIDER_V,
	ITEM_SLIDER_H,
	ITEM_BUTTON,
	ITEM_JOG_WHEEL,

	ITEM_COUNT,
};

struct demo_t {
	uint32_t items[ITEM_COUNT];
};

void event_cb(struct avtka_t *avtka, uint32_t item, float v, void *userdata)
{
	struct demo_t *demo = userdata;
	printf("event on item %d, value %f\n", item, v);
	if(item == demo->items[ITEM_BUTTON] && v > 0.5) {
		for(int i = 0; i < ITEM_COUNT; i++) {
			avtka_item_colour(avtka, demo->items[i], 9);
		}
	}
}

int main()
{
	struct demo_t demo = {0};

	struct avtka_opts_t opts = {
		.w = 360,
		.h = 240,
		.event_callback = event_cb,
		.event_callback_userdata = &demo,
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

	avtka_run(a);

	return 0;
}
