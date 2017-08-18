/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>

#include "avtka.h"

struct avtka_item_t *
avtka_item_create()
{
	return 0;
}

int32_t avtka_item_destroy(struct avtka_item_t *item)
{
	free(item);
	return 0;
}

void avtka_item_visible_set(struct avtka_item_t *item,
			    uint32_t visible)
{
	//item->visible = visible;
}

int32_t avtka_item_reparent(struct avtka_item_t *new_parent,
			    struct avtka_item_t *old_parent)
{
	return 0;
}
