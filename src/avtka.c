/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>

#include "impl.h"
#include "pugl.h"

#include <cairo/cairo.h>

static void on_display(PuglView* view);

static void
on_event(PuglView* view, const PuglEvent* event)
{
	struct avtka_t *a = puglGetHandle(view);
	switch (event->type) {
	case PUGL_KEY_PRESS:
		if (event->key.character == 'q' ||
		    event->key.character == 'Q' ||
		    event->key.character == PUGL_CHAR_ESCAPE) {
			a->quit = 1;
		}
		break;
	case PUGL_BUTTON_PRESS:
		break;
	case PUGL_ENTER_NOTIFY:
		a->entered = true;
		puglPostRedisplay(view);
		break;
	case PUGL_LEAVE_NOTIFY:
		a->entered = false;
		puglPostRedisplay(view);
		break;
	case PUGL_EXPOSE:
		on_display(view);
		break;
	//case PUGL_CLOSE:
		//onClose(view);
		//break;
	default: break;
	}
}

static void
on_display(PuglView* view)
{
	cairo_t* cr = puglGetContext(view);
	struct avtka_t *a = puglGetHandle(view);

#if 0
	if (a->entered)
	else
		cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
#endif

	cairo_set_source_rgb(cr, .12, .12, .12);

	//cairo_rectangle(cr, 0, 0, 1024, 1024);
	cairo_rectangle(cr, 0, 0, a->opts.w, a->opts.h);
	cairo_fill(cr);

	/* iterate all items, calling "draw" on them */
	for (int i = 0; i < a->item_count; i++) {
		uint8_t draw_id = a->items[i].opts.draw;
		if(a->draw[draw_id])
			a->draw[draw_id](a, &a->items[i], cr);
	}
}

struct avtka_t *
avtka_create(const char *window_name, struct avtka_opts_t *opts)
{
	struct avtka_t *ui = calloc(1, sizeof(struct avtka_t));
	if(!ui)
		return 0;

	PuglView *view = puglInit(NULL, NULL);

	puglInitWindowSize  (view, opts->w, opts->h);
	puglInitResizable   (view, true );
	puglInitContextType (view, PUGL_CAIRO);
	puglIgnoreKeyRepeat (view, true );
	puglSetEventFunc    (view, on_event  );
	puglCreateWindow    (view, window_name );
	puglShowWindow      (view);

	ui->pugl = view;
	puglSetHandle       (view, ui);

	ui->draw[AVTKA_DRAW_DIAL] = draw_dial;

	return ui;
}

int32_t
avtka_destroy(struct avtka_t *a)
{
	PuglView *view = a->pugl;
	puglHideWindow(view);
	puglProcessEvents(view);
	free(a);
	return 0;
}

void avtka_iterate(struct avtka_t *a)
{
	puglProcessEvents(a->pugl);
}

void avtka_run(struct avtka_t *a)
{
	while (!a->quit) {
		puglWaitForEvent(a->pugl);
		puglProcessEvents(a->pugl);
	}
	avtka_destroy(a);
}
