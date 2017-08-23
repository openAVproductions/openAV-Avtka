/* This file is part of AVTKA by OpenAV */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "avtka.h"

void event_cb(struct avtka_t *avtka, uint32_t item, float v, void *userdata)
{
	printf("event on item %d, value %f\n", item, v);
}

int main()
{
	struct avtka_opts_t opts = {
		.w = 360,
		.h = 240,
		.event_callback = event_cb,
	};
	struct avtka_t *a = avtka_create("AVTKA v0.1", &opts);

	struct avtka_item_opts_t item = {
		.name = "Dial 1",
		.x = 10, .y = 10, .w = 50, .h = 50,
		.draw = AVTKA_DRAW_DIAL,
		.interact = AVTKA_INTERACT_CLICK,
	};
	uint32_t button1 = avtka_item_create(a, &item);
	item.x = 70;
	snprintf(item.name, sizeof(item.name), "Dial 2");
	uint32_t button2 = avtka_item_create(a, &item);

	printf("items created %d and %d\n", button1, button2);

	avtka_run(a);

	return 0;
}
