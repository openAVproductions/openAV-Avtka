/* This file is part of AVTKA by OpenAV */

#include <stdio.h>

#include "impl.h"

uint32_t
avtka_item_create(struct avtka_t *ui, struct avtka_item_opts_t *o)
{
	ui->items[ui->item_count].opts = *o;
	ui->items[ui->item_count].opts.colour = AVTKA_COL_PRI1;
	ui->items[ui->item_count].col = 0x00007CFF;
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
		int y2 = (it->opts.y + it->opts.h) > y;
		if(x1 && x2 && y1 && y2) {
			return i;
		}
	}
	return 0;
}

void avtka_item_colour(struct avtka_t *a, uint32_t item, uint8_t col)
{
	struct avtka_item_t *it = &a->items[item];
	it->opts.colour = col;
}

void avtka_item_label_show(struct avtka_t *a, uint32_t item, uint32_t show)
{
	struct avtka_item_t *it = &a->items[item];
	it->opts.show_label = show;
}

void avtka_item_value(struct avtka_t *a, uint32_t item, float value)
{
	struct avtka_item_t *it = &a->items[item];
	it->value = value;
}

float avtka_item_value_get(struct avtka_t *a, uint32_t item)
{
	return a->items[item].value;
}

void avtka_item_colour32(struct avtka_t *a, uint32_t item, uint32_t col)
{
	a->items[item].col = col;
}

void avtka_item_value_inc(struct avtka_t *a, uint32_t item, float value)
{
	struct avtka_item_t *it = &a->items[item];
	it->value += value;
	if(it->value > 1.0f) it->value = 1.0f;
	if(it->value < 0.0f) it->value = 0.0f;
}

int32_t avtka_item_destroy(struct avtka_t *ui, uint32_t item_id)
{
	/* de-register colour? */
	return 0;
}

int32_t avtka_item_reparent(struct avtka_item_t *new_parent,
			    struct avtka_item_t *old_parent)
{
	return 0;
}
