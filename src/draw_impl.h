#include "impl.h"

#include <cairo.h>
#include <stdio.h>

#define TRANS (0.2)
#define FILL  (1.0)

struct col_t {
	float r;
	float g;
	float b;
};

static const struct col_t bg[] = {
	{0.40, 0.40, 0.40}, /* BG */
	{0.40, 0.40, 0.40}, /* BG */
};

static const struct col_t fg[] = {
	{0.00, 0.51, 1.00}, /* FG Fade */
	{0.00, 0.51, 1.00}, /* FG */
};

enum COLOURS {
	COL_BG,
	COL_FG_FADE,
	COL_FG,
};

static inline void
draw_label(cairo_t *cr, const char *label,
	   int32_t x_, int32_t y_,
	   int32_t w_, int32_t h_)
{
	cairo_text_extents_t ext;
	cairo_text_extents( cr, label, &ext );
	cairo_move_to( cr, x_+w_/2-ext.width/2., y_+h_+ext.height/2.-6);
	cairo_set_source_rgb( cr, 1,1,1 );
	cairo_show_text( cr, label );
	cairo_stroke(cr);
}

static inline void
set_col(cairo_t *cr, uint32_t c, float a)
{
	cairo_set_source_rgba(cr,
			 ((c & 0x00ff0000) >> 16)/ 255.f,
			 ((c & 0x0000ff00) >>  8) / 255.f,
			 ((c & 0x000000ff)      )/ 255.f,
			 a);
}
