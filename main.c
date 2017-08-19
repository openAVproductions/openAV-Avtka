/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>
#include <unistd.h>

#include "avtka.h"

int main()
{
	struct avtka_opts_t opts = {
		.w = 360,
		.h = 240
	};
	struct avtka_t *a = avtka_create("AVTKA v0.1", &opts);

	struct avtka_item_opts_t item = {
		.name = "Dial",
		.x = 10, .y = 10, .w = 50, .h = 50,
		.draw = AVTKA_DRAW_DIAL,
	};
	uint32_t button1 = avtka_item_create(a, &item);
	item. x = 200;
	uint32_t button2 = avtka_item_create(a, &item);

	avtka_item_visible_set(a, button1, 1);

	avtka_run(a);

	return 0;
}
