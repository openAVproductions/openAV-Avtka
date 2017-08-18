/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>
#include <unistd.h>

#include "avtka.h"

int main()
{
	struct avtka_ui_t *ui = avtka_ui_create("name", 360, 120, NULL);

	avtka_item_visible_set((struct avtka_item_t *)ui, 1);

	sleep(1);

	avtka_ui_destroy(ui);
	return 0;
}
