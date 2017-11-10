/* This file is part of AVTKA by OpenAV */

#include <stdio.h>

#include "impl.h"

#include <cairo/cairo.h>

int32_t
avtka_screen_create(struct avtka_t *a, struct avtka_screen_opts_t *o)
{
	/* add a new screen to the UI here */
	printf("creating screen %d, pixel size %d %d\n", a->screen_count,
	       o->px_x, o->px_y);

	cairo_surface_t *img;

	/* TODO: rework the creation based on HW device capabilities.
	 * Note that the *BINARY* interface of the *HARDWARE CONTROLLER*
	 * should be simulated here. Even if this means looping through
	 * each and every pixel - the same API as is exposed to draw on
	 * the HW must be functioning on the virtual display */
#if 0
	if(o->flags_1bit) {
		img = cairo_image_surface_create(CAIRO_FORMAT_A1, 128, 64);
	}
	else
#endif
	img = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 128, 64);

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
	uint8_t *data = cairo_image_surface_get_data(s);
	return data;
}

