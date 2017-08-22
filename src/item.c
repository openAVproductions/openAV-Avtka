/* This file is part of AVTKA by OpenAV */

#include <stdio.h>

#include "impl.h"

uint32_t
avtka_item_create(struct avtka_t *ui, struct avtka_item_opts_t *o)
{
	ui->items[ui->item_count].opts = *o;
	return ui->item_count++;
}

uint32_t
avtka_item_contact(struct avtka_t *a, uint32_t x, uint32_t y)
{
	for (int i = 0; i < a->item_count; i++) {
		struct avtka_item_t *it = &a->items[i];
		int x1 = it->opts.x < x;
		int x2 = (it->opts.x + it->opts.w) > x;
		int y1 = it->opts.y < y;
		int y2 = (it->opts.y + it->opts.w) > y;
		if(x1 && x2 && y1 && y2) {
			printf("item clicked: %d\n", i);
			return i;
		}
	}
	return 0;
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
