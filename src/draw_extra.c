#include "draw_impl.h"
#include "math.h"

void draw_7seg(struct avtka_t *a, struct avtka_item_t *item, void* cairo)
{
	cairo_t* cr = cairo;
	const int32_t x = item->opts.x;
	const int32_t y = item->opts.y;
	const int32_t w = item->opts.w;
	const int32_t h = item->opts.h;

	const uint8_t nseg = item->opts.params[0] < 3 ?
				item->opts.params[0] : 3;

	float value = item->value;
	if(nseg == 2)
		value = value / 10.f;

	cairo_rectangle( cr, x, y, w, h);
	cairo_set_line_width(cr, 1.f);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
	cairo_set_source_rgba(cr, 0., 0., 0., 1.f);
	cairo_fill_preserve(cr);
	cairo_set_source_rgba(cr, 1,1,1, 0.8f);
	cairo_stroke_preserve(cr);
	cairo_clip( cr );

	uint8_t digits[8] = {1};
	/* cast down to a single int */
	float abs_val = fabsf(value);
	digits[0] = ((int)(abs_val * 9.50f)) % 10;
	digits[1] = ((int)(abs_val * 99.950f)) % 10;
	digits[2] = ((int)(abs_val * 999.50f)) % 10;

	int off = 0;
	if(nseg == 1) off = 0;
	if(nseg == 2) off = 1;
	for(int i = 0; i < nseg; i++) {
		const uint8_t digit = digits[off+i];
		const int32_t w_ = (w / nseg) - 4;
		const int32_t x_ = 2 + x + ((w_+4)*i);
		const int32_t y_ = y + 2;
		const int32_t h_ = h - 4;

		switch(digit) {
		case 0:
			cairo_rectangle( cr, x_, y_, w_, h_);
			break;
		case 1:
			cairo_move_to( cr, x_ + w_, y_);
			cairo_line_to( cr, x_ + w_, y_ + h_);
			break;
		case 2:
			cairo_move_to( cr, x_     , y_);
			cairo_line_to( cr, x_ + w_, y_);
			cairo_line_to( cr, x_ + w_, y_ + (h_/2));
			cairo_line_to( cr, x_     , y_ + (h_/2));
			cairo_line_to( cr, x_     , y_ + h_);
			cairo_line_to( cr, x_ + w_, y_ + h_);
			break;
		case 3:
			cairo_move_to( cr, x_     , y_);
			cairo_line_to( cr, x_ + w_, y_);
			cairo_line_to( cr, x_ + w_, y_ + h_);
			cairo_line_to( cr, x_     , y_ + h_);
			cairo_move_to( cr, x_     , y_ + (h_/2));
			cairo_line_to( cr, x_ + w_, y_ + (h_/2));
			break;
		case 4:
			cairo_move_to( cr, x_     , y_);
			cairo_line_to( cr, x_     , y_ + (h_/2));
			cairo_line_to( cr, x_ + w_, y_ + (h_/2));
			cairo_move_to( cr, x_ + w_, y_);
			cairo_line_to( cr, x_ + w_, y_ + h_);
			break;
		case 5:
			cairo_move_to( cr, x_ + w_, y_);
			cairo_line_to( cr, x_     , y_);
			cairo_line_to( cr, x_     , y_ + (h_/2));
			cairo_line_to( cr, x_ + w_, y_ + (h_/2));
			cairo_line_to( cr, x_ + w_, y_ + h_);
			cairo_line_to( cr, x_     , y_ + h_);
			break;
		case 6:
			cairo_move_to( cr, x_ + w_, y_);
			cairo_line_to( cr, x_     , y_);
			cairo_line_to( cr, x_     , y_ + h_);
			cairo_line_to( cr, x_ + w_, y_ + h_);
			cairo_line_to( cr, x_ + w_, y_ + (h_/2));
			cairo_line_to( cr, x_     , y_ + (h_/2));
			break;
		case 7:
			cairo_move_to( cr, x_     , y_);
			cairo_line_to( cr, x_ + w_, y_);
			cairo_line_to( cr, x_ + w_, y_ + h_);
			break;
		case 8:
			cairo_rectangle( cr, x_, y_, w_, h_);
			cairo_move_to( cr, x_     , y_ + (h_/2));
			cairo_line_to( cr, x_ + w_, y_ + (h_/2));
			break;
		case 9:
			cairo_move_to( cr, x_ + w_, y_ + (h_/2));
			cairo_line_to( cr, x_ + w_, y_);
			cairo_line_to( cr, x_     , y_);
			cairo_line_to( cr, x_     , y_ + (h_/2));
			cairo_line_to( cr, x_ + w_, y_ + (h_/2));
			cairo_line_to( cr, x_ + w_, y_ + h_);
			cairo_line_to( cr, x_     , y_ + h_);
			break;
		default: break;
		};

		set_col(cr, item->col, FILL);
		if(h_ > 10)
			cairo_set_line_width(cr, 2.f);
		cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
		cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
		cairo_stroke(cr);
	};

	/* dot if literally 1.0f was supplied */
	int dot = (value >= 0.9950f);
	if(dot) {
		set_col(cr, ~item->col, FILL);
		cairo_set_line_width(cr, 3.f);
		cairo_rectangle(cr, x, y, 3, 3);
		cairo_stroke(cr);
	}

	/* Negative - sign if value < 0 */
	int negative = (value < 0.0f);
	if(negative) {
		set_col(cr, ~item->col, FILL);
		cairo_set_line_width(cr, 3.f);
		cairo_move_to( cr, x    , y + (h/2)-1);
		cairo_line_to( cr, x + 5, y + (h/2)-1);
		cairo_stroke(cr);
	}
}


void draw_filter(struct avtka_t *a, struct avtka_item_t *item, void* cairo)
{
	cairo_t* cr = cairo;
	const int32_t x_ = item->opts.x;
	const int32_t y_ = item->opts.y;
	const int32_t w_ = item->opts.w;
	const int32_t h_ = item->opts.h;

	float value = item->value;

	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_clip( cr );

	cairo_set_source(cr, a->cols[AVTKA_COL_BG]);
	cairo_rectangle( cr, x_, y_, w_, h_);
	cairo_fill( cr );

	cairo_set_line_width( cr, 18 );
	cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
	cairo_set_source(cr, a->cols[AVTKA_COL_BG_L]);

	// imported from NTK filter widget
	{
		int x = x_;
		int y = y_;
		int w = w_;
		int h = h_;

		float freq = 0;
		int graphType = 0;
		if ( value < 0.45 ) {
			graphType = 0;
			freq = value * 2.f;
		} else if ( value > 0.55 ) {
			graphType = 1;
			freq = (value-0.5)*2.f;
		} else {
			graphType = 2;
		}

		switch( graphType ) {
		case 0 : {
			// draw the cutoff line:
			// move to bottom left, draw line to middle left
			cairo_move_to( cr, x , y + h );
			cairo_line_to( cr, x , y + (h*0.47));

			float cutoff = 0.1 + freq * 0.85;

			// Curve
			cairo_curve_to( cr, x + w * cutoff    , y+(h*0.5)  ,   // control point 1
			                x + w * cutoff    , y+(h * 0.3),   // control point 2
			                x + w * cutoff + 5, y+ h       );  // end of curve 1

			cairo_close_path(cr);

			cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.21 );
			cairo_fill_preserve(cr);
			cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
			cairo_set_line_width(cr, 1.5);
			cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
			cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND);
			cairo_stroke( cr );
		}
		break;
		case 1: {
			// draw the cutoff line:
			float cutoff = 0.95 - (freq*0.8);

			// move to bottom right
			cairo_move_to( cr, x + w, y + h );
			cairo_line_to( cr, x + w, y + (h*0.47));

			// Curve
			cairo_curve_to( cr, x + w - (w*cutoff)    , y+(h*0.5)  ,   // control point 1
			                x + w - (w*cutoff)    , y+(h * 0.3),   // control point 2
			                x + w - (w*cutoff) - 5, y+ h      );   // end of curve 1

			cairo_close_path(cr);

			// stroke
			cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.21 );
			cairo_fill_preserve(cr);
			cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
			cairo_set_line_width(cr, 1.5);
			cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
			cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND);
			cairo_stroke( cr );
		}
		break;
		case 2: {
			// move to bottom right
			cairo_move_to( cr, x + w, y + h );
			cairo_line_to( cr, x + w, y + (h*0.47));
			cairo_line_to( cr, x    , y + (h*0.47));
			cairo_line_to( cr, x    , y + h       );
			cairo_close_path(cr);

			// stroke
			cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.21 );
			cairo_fill_preserve(cr);
			cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
			cairo_set_line_width(cr, 1.5);
			cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
			cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND);
			cairo_stroke( cr );
		}
		break;
		default:
			break;
		}
	}

	// outline
	cairo_set_line_width( cr, 1 );
	cairo_rectangle( cr, x_, y_, w_, h_ );
	cairo_set_source_rgb( cr, 0.72, 0.72, 0.72 );
	cairo_stroke( cr );
}

