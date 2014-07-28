#ifndef BOOKREADER_H
#define BOOKREADER_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/box.h>
#include <string>
#include <vector>

using std::string; 
using std::vector;

class BookArea : public Gtk::DrawingArea
{
public:
  BookArea();
  virtual ~BookArea();

protected:
  //Override default signal handler:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

private:

	double MARGIN_PERCENT = 8.0;

	vector<string> paragraphs;

	void draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
	void draw_header(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height, Glib::ustring title, int pagen);
	void draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int rectangle_width, int rectangle_height);

	};

class BookReader : public Gtk::Box {
	
public:
	BookReader();
	~BookReader();

protected:
	BookArea m_book_area;
};

#endif // BOOKREADER_H
