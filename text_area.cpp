#include "text_area.hpp"
#include "extern_def.h"
#include "recognition.hpp"
#include <glibmm/main.h>

SQL_Connection sql_con;
cv::Mat capture_frame;

TextArea::TextArea()
{
	set_content_width(240);
	set_content_height(240);

	set_draw_func(sigc::mem_fun(*this, &TextArea::on_draw));
	Glib::signal_timeout().connect( sigc::mem_fun(*this, &TextArea::on_timeout), 500 );
}

TextArea::~TextArea()
{
}

void TextArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
	const int rectangle_width = width;
	const int rectangle_height = height;// / 2;

	// Draw a black rectangle
	cr->set_source_rgb(1.0, 1.0, 1.0);
	draw_rectangle(cr, rectangle_width, rectangle_height);

	// and some white text
	cr->set_source_rgb(0.0, 0.0, 0.0);
	draw_text(cr, rectangle_width, rectangle_height);
/*
	// flip the image vertically
	// see http://www.cairographics.org/documentation/cairomm/reference/classCairo_1_1Matrix.html
	// the -1 corresponds to the yy part (the flipping part)
	// the height part is a translation (we could have just called cr->translate(0, height) instead)
	// it's height and not height / 2, since we want this to be on the second part of our drawing
	// (otherwise, it would draw over the previous part)
	Cairo::Matrix matrix(1.0, 0.0, 0.0, -1.0, 0.0, height);

	// apply the matrix
	cr->transform(matrix);

	// white rectangle
	cr->set_source_rgb(1.0, 1.0, 1.0);
	draw_rectangle(cr, rectangle_width, rectangle_height);

	// black text
	cr->set_source_rgb(0.0, 0.0, 0.0);
	draw_text(cr, rectangle_width, rectangle_height);*/
}

void TextArea::draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
	cr->rectangle(0, 0, width, height);
	cr->fill();
}

void TextArea::draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int rectangle_width, int rectangle_height)
{
	// http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
	Pango::FontDescription font;

	font.set_family("Helvetica");

	sql_con.change_employee(recognize(capture_frame));

	std::string context;
	context.append("ФИО:\n");
	context.append(sql_con.get_row(1));
	context.append("\n\nДолжность:\n");
	context.append(sql_con.get_row(2));
	context.append("\n\nДата рождения:\n");
	context.append(sql_con.get_row(3));

	// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
	auto layout = create_pango_layout(context);

	layout->set_font_description(font);

	int text_width;
	int text_height;

	//get the text dimensions (it updates the variables -- by reference)
	layout->get_pixel_size(text_width, text_height);

	// Position the text in the middle
	cr->move_to((rectangle_width-text_width)/2, (rectangle_height-text_height)/2);

	layout->show_in_cairo_context(cr);
}

bool TextArea::on_timeout()
{
	// force our program to redraw the entire clock.
	queue_draw();
	return true;
}
