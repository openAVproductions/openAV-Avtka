/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>
#include <unistd.h>

#include "avtka.h"

int main()
{
	struct avtka_opts_t opts = {
		.w = 320,
		.h = 120
	};
	struct avtka_t *a = avtka_create("AVTKA v0.1", &opts);

	struct avtka_item_opts_t item = {
		.name = "Button 1",
		.x = 10, .y = 10, .w = 50, .h = 22,
		.draw = AVTKA_DRAW_DIAL,
	};
	uint32_t button1 = avtka_item_create(a, &item);
	avtka_item_visible_set(a, button1, 1);

	while(1)
		avtka_iterate(a);

	avtka_destroy(a);
	return 0;
}
