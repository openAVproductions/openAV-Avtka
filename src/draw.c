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
draw_set_col(cairo_t *cr, const struct col_t *c, float a)
{
	cairo_set_source_rgba(cr, c->r, c->g, c->b, a);
}

void draw_button(struct avtka_t *a, struct avtka_item_t *item, void* cairo)
{
	cairo_t* cr = cairo;
	const int32_t x_ = item->opts.x;
	const int32_t y_ = item->opts.y;
	const int32_t w_ = item->opts.w;
	const int32_t h_ = item->opts.h;
	const struct col_t* c = item->value > 0.5 ? fg : bg;

	cairo_rectangle(cr, x_, y_, w_, h_);
	draw_set_col(cr, c, TRANS);
	cairo_fill_preserve(cr);

	cairo_rectangle(cr, x_, y_, w_, h_);
	draw_set_col(cr, c, FILL);
	cairo_stroke(cr);

	char *label = item->opts.name;
	float label_visible = 1;
	if(label_visible) {
		cairo_text_extents_t ext;
		cairo_text_extents( cr, label, &ext );

		cairo_move_to( cr, x_+w_/2-ext.width/2., y_+h_+ext.height/2.-6);
		cairo_set_source_rgb( cr, 1,1,1 );
		cairo_show_text( cr, label );
	}
	cairo_stroke(cr);
}

void draw_slider(struct avtka_t *a, struct avtka_item_t *item, void* c)
{
	cairo_t* cr = c;
	const int32_t x_ = item->opts.x;
	const int32_t y_ = item->opts.y;
	const int32_t w_ = item->opts.w;
	const int32_t h_ = item->opts.h;

	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_set_source_rgba(cr, 0.4, 0.4, 0.4, 1);
	cairo_stroke(cr);

	const int32_t sh = 20;
	float value = item->value;

	/* vertial or horizontal orientation */
	if(h_ > w_) {
		int dy = y_ + ((1-value) * (h_- sh));
		cairo_rectangle( cr, x_ + 1, dy, w_-2, sh);
	}
	else {
		int dx = x_ + ((value) * (w_- sh));
		cairo_rectangle( cr, dx, y_ + 1, sh, h_ - 2);
	}

	cairo_set_source_rgba(cr, 0.0, 0.5, 1, 0.3);
	cairo_fill_preserve(cr);
	cairo_set_source_rgba(cr, 0.0, 0.5, 1, 1);
	cairo_stroke(cr);
}

void draw_dial(struct avtka_t *a, struct avtka_item_t *item, void* c)
{
	cairo_t* cr = c;
	const int32_t x_ = item->opts.x;
	const int32_t y_ = item->opts.y;
	const int32_t w_ = item->opts.w;
	const int32_t h_ = item->opts.h;

	float value = item->value;
	float label_visible = 1;
	char *label = item->opts.name;

	cairo_set_source_rgba(cr, 0.8, 0.8, 0.8, 1.0);
	cairo_arc(cr, x_+w_/2,y_+h_/2,  w_/2.f - 8, 2.46, 2.46 + 4.54 * 1);
	cairo_set_line_width(cr, w_ / 20.f);
	cairo_stroke(cr);

	cairo_move_to( cr, x_, y_);
	cairo_set_source_rgba(cr, 0., 0.51, 1, 1.0);
	cairo_new_sub_path( cr );
	cairo_arc(cr, x_+w_/2,y_+h_/2,  w_/2.f - 8, 2.46, 2.46 + 4.54 * value );
	cairo_set_line_width(cr, w_ / 7.f);
	cairo_stroke(cr);

	if( label_visible ) {
		cairo_text_extents_t ext;
		cairo_text_extents( cr, label, &ext );

		cairo_move_to( cr, x_+w_/2-ext.width/2., y_+h_+ext.height/2.-6);
		cairo_set_source_rgb( cr, 1,1,1 );
		cairo_show_text( cr, label );
	}
	cairo_stroke(cr);
}

