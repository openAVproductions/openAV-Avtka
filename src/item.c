/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>

#include "impl.h"

uint32_t
avtka_item_create(struct avtka_t *ui, struct avtka_item_opts_t *o)
{
	ui->items[ui->item_count].opts = *o;
	ui->item_count++;
	return ui->item_count;
}

int32_t avtka_item_destroy(struct avtka_t *ui, uint32_t item_id)
{
	return 0;
}

void avtka_item_visible_set(struct avtka_t *a,
			    uint32_t item,
			    uint32_t visible)
{
	//item->visible = visible;
}

int32_t avtka_item_reparent(struct avtka_item_t *new_parent,
			    struct avtka_item_t *old_parent)
{
	return 0;
}
