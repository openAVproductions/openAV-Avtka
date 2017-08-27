/* This file is part of AVTKA by OpenAV */

#include <stdio.h>

#include "impl.h"

int32_t avtka_interact_press(struct avtka_t *a, uint32_t item,
			     uint32_t x, uint32_t y)
{
	switch(a->items[item].opts.interact) {
	case AVTKA_INTERACT_CLICK:
		/* toggle value */
		a->items[item].value = !a->items[item].value;
		/* perform callback */
		if(a->opts.event_callback) {
			a->opts.event_callback(a, item,
				a->items[item].value,
				a->opts.event_callback_userdata);
		}
		/* redraw */
		return 1;
	case AVTKA_INTERACT_DRAG_V:
		/* store the verital click position */
		a->items[item].clicked = y;
		a->clicked_item = item;
		/* no redraw yet */
		return 0;
	case AVTKA_INTERACT_DRAG_H:
		/* store the verital click position */
		a->items[item].clicked = x;
		a->clicked_item = item;
		/* no redraw yet */
		return 0;
	default: break;
	}

	return 0;
}

int32_t avtka_interact_motion(struct avtka_t *a, uint32_t item,
			      int32_t x, int32_t y)
{
	switch(a->items[item].opts.interact) {
	case AVTKA_INTERACT_DRAG_V: {
		float tmp = ( ((int32_t)y) - a->items[item].clicked);
		float val_offset = -(tmp / 150.f);
		tmp = a->items[item].value + val_offset;
		tmp = tmp > 1.0f ? 1.0 : tmp;
		tmp = tmp < 0.0f ? 0.0 : tmp;
		a->items[item].value = tmp;
		a->items[item].clicked = y;

		if(a->opts.event_callback) {
			a->opts.event_callback(a, item,
				a->items[item].value,
				a->opts.event_callback_userdata);
		}
		} return 1;
	case AVTKA_INTERACT_DRAG_H: {
		float tmp = ( ((int32_t)x) - a->items[item].clicked);
		float val_offset = (tmp / 150.f);
		tmp = a->items[item].value + val_offset;
		tmp = tmp > 1.0f ? 1.0 : tmp;
		tmp = tmp < 0.0f ? 0.0 : tmp;
		a->items[item].value = tmp;
		a->items[item].clicked = x;

		if(a->opts.event_callback) {
			a->opts.event_callback(a, item,
				a->items[item].value,
				a->opts.event_callback_userdata);
		}
		} return 1;
	default: break;
	}

	return 0;
}
