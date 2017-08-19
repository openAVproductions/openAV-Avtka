/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>
#include <unistd.h>

#include "avtka.h"

int main()
{
	struct avtka_t *a = avtka_create("AVTKA v0.1", 360, 120, NULL);

	//uint32_t button1 = avtka_item_create(a, "button1", 

	avtka_item_visible_set(a, 0, 1);

	sleep(1);

	avtka_destroy(a);
	return 0;
}
