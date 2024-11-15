#ifndef GTKMM_TEXTAREA_H
#define GTKMM_TEXTAREA_H

#include <gtkmm/drawingarea.h>

class TextArea : public Gtk::DrawingArea
{
public:
	TextArea();
	virtual ~TextArea();

protected:
	void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
	bool on_timeout();

private:
	void draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
	void draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int rectangle_width, int rectangle_height);

};

#endif // GTKMM_TEXTAREA_HPP
