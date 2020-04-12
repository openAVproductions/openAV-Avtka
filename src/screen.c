/* This file is part of AVTKA by OpenAV */

#include <stdio.h>

#include "impl.h"

#include <cairo/cairo.h>

int32_t
avtka_screen_create(struct avtka_t *a, struct avtka_screen_opts_t *o)
{
	cairo_surface_t *img;
	/* TODO: rework the creation based on HW device capabilities.
	 * Note that the *BINARY* interface of the *HARDWARE CONTROLLER*
	 * should be simulated here. Even if this means looping through
	 * each and every pixel - the same API as is exposed to draw on
	 * the HW must be functioning on the virtual display */
	if(o->flags_px_bit_1) {
		img = cairo_image_surface_create(CAIRO_FORMAT_A1,
						 o->px_x, o->px_y);
	} if(o->flags_px_rgb_16_565) {
		img = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565,
						 o->px_x, o->px_y);
	} if(o->flags_px_argb_32) {
		img = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
						 o->px_x, o->px_y);
	} if(o->flags_px_rgb_24) {
		img = cairo_image_surface_create(CAIRO_FORMAT_RGB24,
						 o->px_x, o->px_y);
	} else {
		AVTKA_WARN(a, "Failed: no pixel format set. flags_px_* all %d\n", 0);
		return -1;
	}

	/* fill with black for starting */
	cairo_t *cr = cairo_create(img);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_rectangle(cr, 0, 0, o->px_x, o->px_y);
	cairo_fill(cr);
	cairo_surface_flush(img);
	cairo_destroy(cr);

	int screen_id = a->screen_count;
	a->screen_surfaces[screen_id] = img;
	a->screen_opts[screen_id] = *o;

	a->screen_count++;

	return screen_id;
}

uint8_t *
avtka_screen_get_data_ptr(struct avtka_t *a, uint16_t screen_id)
{
	cairo_surface_t *s = a->screen_surfaces[screen_id];
	if(!s)
		return 0;

	uint8_t *data = cairo_image_surface_get_data(s);
	return data;
}

