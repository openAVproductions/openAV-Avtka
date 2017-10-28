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

void draw_button(struct avtka_t *a, struct avtka_item_t *item, void* cairo)
{
	cairo_t* cr = cairo;
	const int32_t x_ = item->opts.x;
	const int32_t y_ = item->opts.y;
	const int32_t w_ = item->opts.w;
	const int32_t h_ = item->opts.h;

	cairo_rectangle(cr, x_, y_, w_, h_);
	set_col(cr, item->col, TRANS);
	cairo_fill_preserve(cr);

	set_col(cr, item->col, FILL);
	cairo_stroke(cr);

	if(item->opts.show_label)
		draw_label(cr, item->opts.name, x_, y_, w_, h_);
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
	cairo_set_source(cr, a->cols[AVTKA_COL_BG_L]);
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

	cairo_set_source(cr, a->cols[item->opts.colour+2]);
	cairo_fill_preserve(cr);
	cairo_set_source(cr, a->cols[item->opts.colour]);
	cairo_stroke(cr);

	if(item->opts.show_label)
		draw_label(cr, item->opts.name, x_, y_, w_, h_);
}

void draw_led_strip(struct avtka_t *a, struct avtka_item_t *item, void* c)
{
	cairo_t* cr = c;
	const int32_t x_ = item->opts.x;
	const int32_t y_ = item->opts.y;
	const int32_t w_ = item->opts.w;
	const int32_t h_ = item->opts.h;
	float value = item->value;

	uint8_t num_segs = item->opts.params[0];
	//uint8_t orange   = item->opts.params[2];
	//printf("%s %d: v = %f\n", __func__, num_segs, value);
	if(!num_segs) {
		AVTKA_WARN(a, "num segs == %d, returning\n", num_segs);
		return;
	}

	/* TODO: add support for horizontal / vertial */
	uint32_t led_dist = h_ / (num_segs);
	uint32_t led_height = h_ / (num_segs + 2);

	uint32_t delta = (led_dist - led_height);

	cairo_set_line_width(cr, 1.5);
	for(int i = 0; i < num_segs; i++) {
		int y = (y_ + h_) - (led_dist * (i+1)) + (delta/2);
		cairo_rectangle( cr, x_ + 1, y, w_ - 2, led_height);
		if(i < value * num_segs) {
			cairo_set_source_rgba(cr, 0.0, 0.5, 1, 0.8);
			cairo_fill(cr);
		} else {
			cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
			cairo_stroke(cr);
		}
	}
	cairo_stroke(cr);

	if(item->opts.show_label)
		draw_label(cr, item->opts.name, x_, y_, w_, h_);
}

void draw_dial(struct avtka_t *a, struct avtka_item_t *item, void* c)
{
	cairo_t* cr = c;
	const int32_t x_ = item->opts.x;
	const int32_t y_ = item->opts.y;
	const int32_t w_ = item->opts.w;
	const int32_t h_ = item->opts.h;

	float value = item->value;

	cairo_set_source(cr, a->cols[AVTKA_COL_BG_L]);
	cairo_arc(cr, x_+w_/2,y_+h_/2,  w_/2.f - 8, 2.46, 2.46 + 4.54 * 1);
	cairo_set_line_width(cr, w_ / 20.f);
	cairo_stroke(cr);

	cairo_move_to( cr, x_, y_);
	cairo_set_source(cr, a->cols[AVTKA_COL_PRI1]);
	cairo_new_sub_path( cr );
	/* Crude NaN detection - cairo assert trips if it gets a NaN in arc() */
	if(value != value) {
		value = 0.f;
	}

	cairo_arc(cr, x_+w_/2,y_+h_/2,  w_/2.f - 8, 2.46, 2.46 + 4.54 * value );
	cairo_set_line_width(cr, w_ / 7.f);
	cairo_stroke(cr);

	if(item->opts.show_label)
		draw_label(cr, item->opts.name, x_, y_, w_, h_);
	cairo_stroke(cr);
}


void draw_jog_wheel(struct avtka_t *a, struct avtka_item_t *item, void* c)
{
	cairo_t* cr = c;
	const int32_t x_ = item->opts.x;
	const int32_t y_ = item->opts.y;
	const int32_t w_ = item->opts.w;
	const int32_t h_ = item->opts.h;

	float value = item->value;

#if 0
	cairo_rectangle(cr, x_, y_, w_, h_);
	cairo_set_source_rgba(cr, 0.0, 1, 0.0, 0.3);
	cairo_stroke(cr);
#endif

	cairo_set_source(cr, a->cols[AVTKA_COL_BG_L]);
	cairo_arc(cr, x_+w_/2,y_+h_/2,  w_/2.f - 8, 0, 6.3);
	cairo_set_line_width(cr, w_ / 20.f);
	cairo_stroke(cr);

	cairo_move_to( cr, x_, y_);
	cairo_stroke(cr);
	const float full = (3.15*2);
	float v = full * value;
	const float pi_4 = (3.15*2)/4;
	cairo_arc(cr, x_+w_/2,y_+h_/2,  w_/2.f - 8, -pi_4*1.5 + v, -pi_4 + v);
	cairo_line_to( cr, x_ + (w_/2), y_ + (h_/2));
	cairo_set_source(cr, a->cols[AVTKA_COL_PRI1]);
	cairo_set_line_width(cr, w_ / 7.5f);
	cairo_stroke(cr);

	v += 0.5 * full;

	cairo_move_to( cr, x_, y_);
	cairo_new_sub_path( cr );
	cairo_arc(cr, x_+w_/2,y_+h_/2,  w_/2.f - 8, -pi_4*1.5 + v, -pi_4 + v);
	//cairo_arc(cr, x_+w_/2,y_+h_/2,  w_/2.f - 8, pi_4/4 + v, pi_4 + v);
	cairo_line_to( cr, x_ + (w_/2), y_ + (h_/2));
	cairo_stroke(cr);

	if(item->opts.show_label)
		draw_label(cr, item->opts.name, x_, y_, w_, h_);
}

