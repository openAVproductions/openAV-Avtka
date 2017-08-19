/* This file is part of AVTKA by OpenAV */

#include <stdlib.h>

#include "avtka.h"
#include "pugl.h"

struct avtka_item_t {
	/* tailq for sub items */
};

struct avtka_t {
	/* a ui pointer can be treated as an item */
	struct avtka_item_t item;

	/* implementation details */
	PuglView* pugl;
};

struct avtka_t *
avtka_create(const char *window_name, uint32_t w_, uint32_t h_,
	     struct avtka_opts_t *opts)
{
	struct avtka_t *ui = calloc(1, sizeof(struct avtka_t));
	if(!ui)
		return 0;

	PuglView *view = puglInit(NULL, NULL);

	puglInitWindowSize  (view, w_, h_ );
	puglInitResizable   (view, true );
	puglInitContextType (view, PUGL_CAIRO);
	puglIgnoreKeyRepeat (view, true );

#if 0
	puglSetEventFunc    (view, UI::onEvent  );
	puglSetDisplayFunc  (view, UI::onDisplay);
	puglSetCloseFunc    (view, UI::onClose  );
	puglSetMotionFunc   (view, UI::onMotion );
	puglSetReshapeFunc  (view, UI::onReshape);
#endif

	puglCreateWindow    (view, window_name );
	puglShowWindow      (view);

	ui->pugl = view;
	puglSetHandle       (view, ui);

	puglProcessEvents(view);

	return ui;
}

int32_t
avtka_destroy(struct avtka_t *ui)
{
	PuglView *view = ui->pugl;
	puglHideWindow(view);
	puglProcessEvents(view);
	free(ui);
	return 0;
}
