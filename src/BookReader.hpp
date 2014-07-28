#ifndef BOOKREADER_H
#define BOOKREADER_H

#include <gtkmm/drawingarea.h>

class BookReader : public Gtk::DrawingArea
{
public:
  BookReader();
  virtual ~BookReader();

protected:
  //Override default signal handler:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

private:

  double MARGIN_PERCENT = 6.0;

  void draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
  void draw_header(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height, Glib::ustring title, int pagen);
  void draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int rectangle_width, int rectangle_height);

};

#endif // BOOKREADER_H
