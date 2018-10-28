/* This file is part of AVTKA by OpenAV */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "avtka.h"

#define ITEM_COUNT 32

struct dyn_t {
	uint32_t items[ITEM_COUNT];
	uint8_t cols[2];
	uint8_t col_counter;
};

static int static_item_x = 1;

void event_cb(struct avtka_t *avtka, uint32_t item_id, float v, void *userdata)
{
	struct dyn_t *dyn = userdata;
	printf("event on item %d, value %f\n", item_id, v);

	struct avtka_item_opts_t item = {
		.name = "thing",
		.x = static_item_x++ * 60, .y = 10, .w = 50, .h = 50,
		.draw = AVTKA_DRAW_DIAL,
		.interact = AVTKA_INTERACT_DRAG_V,
	};
	snprintf(item.name, sizeof(item.name), "Slider %d", static_item_x);
	avtka_item_create(avtka, &item);
}

static uint32_t done;

void sighandler(int signal)
{
	done = 1;
}

int main()
{
	signal(SIGINT, &sighandler);

	struct dyn_t dyn;
	memset(&dyn, 0, sizeof(struct dyn_t));
	dyn.cols[0] = 5;
	dyn.cols[1] = 9;

	struct avtka_opts_t opts = {
		.w = 360,
		.h = 240,
		.event_callback = event_cb,
		.event_callback_userdata = &dyn,
		/* comment to have a fixed size window */
		.resizeable = 1,

		/* Uncomment to show red square around partial redraw */
		//.debug_redraws = 1,

		/* Uncomment to NOT show an X11 window, instead using an
		 * offscreen buffer. Useful for embedded use-cases */
		//.offscreen_only = 1,
	};
	struct avtka_t *a = avtka_create("AVTKA v0.1", &opts);
	if(!a) {
		printf("failed to create avtka instance, quitting\n");
		return 0;
	}

	struct avtka_item_opts_t item = {
		.name = "Dial 1",
		.x = 10, .y = 10, .w = 50, .h = 50,
		.draw = AVTKA_DRAW_BUTTON,
		.interact = AVTKA_INTERACT_CLICK,
	};
	dyn.items[0] = avtka_item_create(a, &item);

	/* iterate once to draw screen, then take screenshot */
	avtka_iterate(a);
	int ret = avtka_take_screenshot(a, "screen.png");
	if(ret)
		printf("screenshot error, returned %d\n", ret);

#if RUN_FOREVER
	/* signal handler won't work here */
	avtka_run(a);
#else
	uint32_t dx;
	uint32_t dy;
	uint32_t dw;
	uint32_t dh;
	while(!done) {
		avtka_iterate(a);
		avtka_redraw_get_damaged_area(a, &dx, &dy, &dw, &dh);
	}

	avtka_destroy(a);

#endif
	return 0;
}
