/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>
#include <unistd.h>

#include "avtka.h"

int main()
{
	struct avtka_opts_t opts = {
		.w = 820,
		.h = 420
	};
	struct avtka_t *a = avtka_create("AVTKA v0.1", &opts);

	struct avtka_item_opts_t item = {
		.name = "Button 1",
		.x = 0, .y = 0, .w = 30, .h = 30,
		.draw = AVTKA_DRAW_DIAL,
	};
	uint32_t button1 = avtka_item_create(a, &item);
	avtka_item_visible_set(a, button1, 1);

	avtka_run(a);

	return 0;
}
