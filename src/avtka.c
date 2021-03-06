/* This file is part of AVTKA by OpenAV */

#include <stdio.h>

#include "impl.h"
#include "pugl/pugl.h"

#include <cairo/cairo.h>

static void pugl_on_display(PuglView* view);

static inline void
item_damage_and_redisplay(struct avtka_t *a, uint32_t item, PuglView *view)
{
	item_damage(a, item);
	puglPostRedisplay(view);
}

static void
on_event(PuglView* view, const PuglEvent* event)
{
	struct avtka_t *a = puglGetHandle(view);

	float scale_inv = 1 / a->rescale;

	int handled = 1;

	switch (event->type) {
	case PUGL_KEY_PRESS:
		if (event->key.character == 'q' ||
		    event->key.character == 'Q' ||
		    event->key.character == PUGL_CHAR_ESCAPE) {
			a->quit = 1;
		}
		break;
	case PUGL_BUTTON_PRESS: {
		uint32_t x = event->button.x * scale_inv;
		uint32_t y = event->button.y * scale_inv;
		uint32_t item = avtka_item_contact(a, x, y);

		uint8_t edit_mode_flags = (PUGL_MOD_SHIFT | PUGL_MOD_CTRL);
		uint8_t edit_mode = (event->button.state == edit_mode_flags);
		uint8_t right_click = (event->button.button == 3);

		if(edit_mode && right_click) {
			//printf("ctlr-shift-r_click - opening editor\n");
			a->edit_mode = !a->edit_mode;
		} else {
			int32_t redraw = avtka_interact_press(a, item, x, y);
			if(redraw)
				item_damage_and_redisplay(a, item, view);
			}
		} break;
	case PUGL_BUTTON_RELEASE: {
		if(a->clicked_item == 0)
			AVTKA_INFO(a, "Button release was %d\n",
				   a->clicked_item);
		uint32_t x = event->button.x * scale_inv;
		uint32_t y = event->button.y * scale_inv;
		int32_t redraw = avtka_interact_release(a, a->clicked_item,
							x, y);
		if(redraw)
			item_damage_and_redisplay(a, a->clicked_item, view);
		a->clicked_item = 0;
		} break;
	case PUGL_MOTION_NOTIFY: {
		if(a->clicked_item) {
			int x = event->motion.x * scale_inv;
			int y = event->motion.y * scale_inv;
			int32_t redraw = avtka_interact_motion(a,
							       a->clicked_item,
							       x, y);
			if(redraw)
				item_damage_and_redisplay(a, a->clicked_item,
							  view);
		}
		handled = 0;
		} break;
	case PUGL_ENTER_NOTIFY:
		a->entered = true;
		puglPostRedisplay(view);
		break;
	case PUGL_LEAVE_NOTIFY:
		a->entered = false;
		puglPostRedisplay(view);
		break;
	case PUGL_CONFIGURE: {
		int width;
		int height;
		puglGetSize(view, &width, &height);
		float rw = ((float)width) / a->opts.w;
		float rh = ((float)height) / a->opts.h;
		a->rescale = rw >= rh ? rh : rw;
		} break;
	case PUGL_EXPOSE:
		pugl_on_display(view);
		break;
	case PUGL_CLOSE:
		a->quit = 1;
		break;
	default:
		handled = 0;
		break;
	}

	a->rev += handled;
}

static void
avtka_on_display(struct avtka_t *a, cairo_t *cr)
{
	/* resize the whole cairo drawing, to draw bigger */
	if(a->rescale < 0.001f)
		printf("%s: warning scale very small: %f\n", __func__,
		       a->rescale);
	cairo_scale(cr, a->rescale, a->rescale);

	/* draw background to fill "official aspect ratio area" */
	cairo_set_source_rgb(cr, .12, .12, .12);
	cairo_rectangle(cr, 0, 0, a->opts.w, a->opts.h);
	cairo_fill(cr);

	/* paint screens to the UI */
	for (int i = 0; i < a->screen_count; i++) {
		cairo_surface_t *s = a->screen_surfaces[i];
		int x = a->screen_opts[i].x;
		int y = a->screen_opts[i].y;

		float screen_rescale_x = (float)a->screen_opts[i].w /
			a->screen_opts[i].px_x;
		float screen_rescale_y = (float)a->screen_opts[i].h /
			a->screen_opts[i].px_y;

		/* translate/scale the screen by the device's pixel / mm */
		cairo_save(cr);
		cairo_translate(cr, x, y);
		cairo_scale(cr,screen_rescale_x, screen_rescale_y);

		/* outline the screen */
		cairo_t *scr = cairo_create(s);
		cairo_set_source_rgba(scr, 1, 1, 1, 0.8);
		cairo_rectangle(scr, 0, 0, a->screen_opts[i].px_x,
				a->screen_opts[i].px_y);
		cairo_stroke(scr);
		cairo_surface_flush(s);
		cairo_destroy(scr);

		cairo_set_source_surface(cr, s, 0, 0);
		cairo_paint(cr);
		cairo_restore(cr);
	}

	/* iterate all items, calling "draw" on them */
	for (int i = 0; i < a->item_count; i++) {
		uint8_t draw_id = a->items[i].opts.draw;
		if(a->draw[draw_id]) {
			cairo_save(cr);
			a->draw[draw_id](a, &a->items[i], cr);
			cairo_restore(cr);
		}

#if 0
		/* For testing each draw iteration output */
		char buf[64];
		snprintf(buf, sizeof(buf), "item_%d.png", i);
		cairo_surface_flush(a->cairo_surface);
		cairo_surface_write_to_png(a->cairo_surface, buf);
#endif
	}

	if(a->opts.debug_redraws) {
		cairo_set_source_rgb(cr, 1, 0, 0);
		int w = a->damage_w - a->damage_x;
		int h = a->damage_h - a->damage_y;
		cairo_rectangle(cr, a->damage_x, a->damage_y,
				w, h);
		cairo_stroke(cr);
		/*
		printf("final damage rect: %d,%d %d,%d\n",
				a->damage_x, a->damage_y, w, h);
		*/
	}

	if(a->edit_mode) {
		cairo_set_source_rgba(cr, 0, 0x7C, 0xFF, 0.2);
		cairo_rectangle(cr, 20, 20, a->opts.w - 40, a->opts.h - 40);
		cairo_stroke(cr);
	}

	cairo_identity_matrix(cr);
}

static void
pugl_on_display(PuglView* view)
{
	struct avtka_t *a = puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	avtka_on_display(a, cr);
}

uint8_t
avtka_register_colour(struct avtka_t *avtka,
		      float r, float g, float b, float a)
{
	/* pre-compute pattern_t, save into internal LUT. See
	 * AVTKA_COLOURS enum in impl.h */
	float o = 0.2;
	uint8_t i = avtka->cols_used++;
	avtka->cols[(i*4)+0] = cairo_pattern_create_rgba(r-o, g-o, b-o, a);
	avtka->cols[(i*4)+1] = cairo_pattern_create_rgba(r, g, b, a);
	avtka->cols[(i*4)+2] = cairo_pattern_create_rgba(r+o, g+o, b+o, a);
	avtka->cols[(i*4)+3] = cairo_pattern_create_rgba(r, g, b, 0.2);
	uint8_t col = (i * 4) + 1;
	return col;
}

static void
avtka_colours_init(struct avtka_t *avtka)
{
	static uint32_t defaults[] = {
		0xff515151, /* BG */
		//0xcc0082ff, /* PRI1 */
		0xefffffff, /* PRI1 - white */
		0xffff0000, /* RED */
	};
#define NUM_DEF_COLS (sizeof(defaults) / sizeof(defaults[0]))

	int i;
	for(i = 0; i < NUM_DEF_COLS; i++) {
		float a = ((defaults[i]           ) >> 24) / 255.f;
		float r = ((defaults[i] & 0xff0000) >> 16) / 255.f;
		float g = ((defaults[i] & 0x00ff00) >>  8) / 255.f;
		float b = ((defaults[i] & 0x0000ff) >>  0) / 255.f;
		avtka_register_colour(avtka, r, g, b, a);
	}
	avtka->cols_used = i;
}

static struct avtka_t *
avtka_create_impl(const char *window_name, struct avtka_opts_t *opts)
{
	BUILD_BUG_ON(sizeof(struct avtka_opts_t) != 64);
	BUILD_BUG_ON(sizeof(struct avtka_item_opts_t) != 36);

	struct avtka_t *ui = calloc(1, sizeof(struct avtka_t));
	if(!ui) {
		AVTKA_ERROR(0, "failed to allocate memory: ui %p\n", ui);
		return 0;
	}

	/* ENV variables override application opts */
	char *avtka_debug = getenv("AVTKA_DEBUG");
	if(avtka_debug) {
		int debug_level = atoi(avtka_debug);
		ui->opts.debug_level = debug_level;
		if(debug_level >= AVTKA_DEBUG_INFO)
			AVTKA_INFO(ui, "debug level: %d\n", debug_level);
	}

	if(!window_name || !opts) {
		AVTKA_ERROR(ui, "invalid window name or NULL opts: name: '%s' opts: %p\n",
			    window_name, opts);
		goto fail;
	}

	if(opts->w == 0 || opts->h == 0) {
		AVTKA_ERROR(ui, "invalid window parameters: w: %d, h: %d\n",
			    opts->w, opts->h);
		goto fail;
	}

	ui->opts = *opts;

	ui->draw[AVTKA_DRAW_BUTTON] = draw_button;
	ui->draw[AVTKA_DRAW_DIAL] = draw_dial;
	ui->draw[AVTKA_DRAW_JOG_WHEEL] = draw_jog_wheel;
	ui->draw[AVTKA_DRAW_LED_STRIP] = draw_led_strip;
	ui->draw[AVTKA_DRAW_SLIDER] = draw_slider;
	ui->draw[AVTKA_DRAW_FILTER] = draw_filter;
	ui->draw[AVTKA_DRAW_7_SEG] = draw_7seg;

	/* item 0 is used as "no item", so start from 1 */
	ui->item_count = 1;
	/* default drag sensitivity: 150px native size UI from 0 to 1 */
	ui->drag_sensitivity = 150.f;

	avtka_colours_init(ui);

	return ui;
fail:
	if(ui)
		free(ui);
	return 0;
}

/* sets up a PUGL context for this AVKTA instance. This is not required
 * to enable controller screens use Cairo directly - without any graphics
 * backend like X11 to handle events/redraws.
 */
void
avtka_init_pugl(struct avtka_t *ui, const char *window_name)
{
	PuglView *view = puglInit(NULL, NULL);

	struct avtka_opts_t *opts = &ui->opts;

	/* embed if requrested */
	if(opts->native_parent != 0)
		puglInitWindowParent(view, opts->native_parent);

	puglInitWindowSize  (view, opts->w, opts->h);
	puglInitResizable   (view, opts->resizeable);
	puglInitContextType (view, PUGL_CAIRO);
	puglIgnoreKeyRepeat (view, true );
	puglSetEventFunc    (view, on_event);
	puglCreateWindow    (view, window_name);
	puglShowWindow      (view);
	puglSetHandle       (view, ui);
	ui->pugl = view;

	ui->cairo_surface = puglGetContextSurface(ui->pugl);
	ui->cairo_context = puglGetContext(ui->pugl);
}

int
avtka_init_raw_cairo(struct avtka_t *a)
{
	/* TODO: think about surface formats - support 565 here?
	 * Pro: cairo handles 565 conversion, con; performance of drawing
	 */
	a->cairo_surface = cairo_image_surface_create(
						      CAIRO_FORMAT_ARGB32,
						      //CAIRO_FORMAT_RGB16_565,
						      a->opts.w,
						      a->opts.h);
	cairo_status_t ret = cairo_surface_status(a->cairo_surface);
	if(ret != CAIRO_STATUS_SUCCESS) {
		printf("error creating surface, cairo returns %s\n",
			cairo_status_to_string(ret));
		return -1;
	}

	a->cairo_context = cairo_create(a->cairo_surface);
	ret = cairo_status(a->cairo_context);
	if(ret != CAIRO_STATUS_SUCCESS) {
		printf("error creating context, cairo returns %s\n",
			cairo_status_to_string(ret));
		return -1;
	}

	/* as its a offscreen only, just set scale to 1.0f */
	a->rescale = 1.0f;

	return 0;
}

struct avtka_t *
avtka_create(const char *window_name, struct avtka_opts_t *opts)
{
	struct avtka_t *ui = avtka_create_impl(window_name, opts);
	if(!ui)
		return 0;

	if(ui->opts.offscreen_only)
		avtka_init_raw_cairo(ui);
	else
		avtka_init_pugl(ui, window_name);

	return ui;
}

void
avtka_visible(struct avtka_t *a, uint8_t visible)
{
	if(a->opts.offscreen_only)
		return;

	if(visible)
		puglShowWindow(a->pugl);
	else {
		puglHideWindow(a->pugl);
		printf("hiding window...\n");
	}

	avtka_redraw(a);
}

avtka_native_t
avtka_get_native_handle(struct avtka_t *a)
{
	return puglGetNativeWindow(a->pugl);
}

void *
avtka_get_cairo_surface(struct avtka_t *a)
{
	return a->cairo_surface;
}

int avtka_take_screenshot(struct avtka_t *a, const char *filename)
{
	if(!filename)
		return -1;

	cairo_status_t ret = cairo_surface_write_to_png(a->cairo_surface,
							filename);
	if(ret != CAIRO_STATUS_SUCCESS) {
		printf("%s: error taking screenshot, cairo returns %s\n",
		       __func__, cairo_status_to_string(ret));
		return -2;
	}

	return 0;
}

void
avtka_redraw(struct avtka_t *a)
{
	if(!a->opts.offscreen_only)
		puglPostRedisplay(a->pugl);
	/* raw cairo will redraw anyway */
}

void
avtka_redraw_get_damaged_area(struct avtka_t *a,
			      uint32_t *x, uint32_t *y,
			      uint32_t *w, uint32_t *h)
{
	*x = a->damage_x;
	*y = a->damage_y;

	*w = a->damage_w - a->damage_x;
	*h = a->damage_h - a->damage_y;

	if(a->damage_rev != a->rev)
		a->damage_rev = a->rev;

	/* set to "extremes" of wrong, so any item is counted */
	a->damage_x = a->opts.w;
	a->damage_y = a->opts.h;
	a->damage_w = 0;
	a->damage_h = 0;
}

int32_t
avtka_destroy(struct avtka_t *a)
{
	PuglView *view = a->pugl;
	if(view) {
		puglHideWindow(view);
		puglDestroy(view);
	} else {
		cairo_surface_destroy(a->cairo_surface);
		cairo_destroy(a->cairo_context);
	}

	/* free cairo pattern_t's used for colours here */
	for(int i = 0; i < a->cols_used * 4; i++)
		cairo_pattern_destroy(a->cols[i]);

	for(int i = 0; i < a->screen_count; i++)
		cairo_surface_destroy(a->screen_surfaces[i]);

	free(a);
	return 0;
}

void
avtka_iterate(struct avtka_t *a)
{
	if(a->pugl) {
		puglProcessEvents(a->pugl);
		return;
	}

	/* manually iter events and updates, redraw the cairo backend */
	avtka_on_display(a, a->cairo_context);
}

void
avtka_run(struct avtka_t *a)
{
	while (!a->quit) {
		puglWaitForEvent(a->pugl);
		puglProcessEvents(a->pugl);
	}
	avtka_destroy(a);
}
