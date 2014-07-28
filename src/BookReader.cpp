#include "BookReader.hpp"

#include <iostream>

using namespace std;

BookArea::BookArea()
{
	cout << "Created" << endl; 
}

BookArea::~BookArea()
{
}

bool BookArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	cout << "Window is " << width << "x" << height << endl;


	const int rectangle_width = width;
	const int rectangle_height = height;

	//cr->set_antialias(ANTIALIAS_BEST);

	cr->set_source_rgb(1.0, 1.0, 1.0);
	draw_rectangle(cr, rectangle_width, rectangle_height);

	cr->set_source_rgb(0.5, 0.5, 0.5);	
	draw_header(cr, rectangle_width, rectangle_height, "This is the book title", 21);

	cr->set_source_rgb(0.1, 0.1, 0.1);
	draw_text(cr, rectangle_width, rectangle_height);

	return true;
}

void BookArea::draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr,
                            int width, int height)
{
	
	cr->rectangle(0, 0, width, height);
	cr->fill();
	cr->rectangle(0, 0, width, height);
	cr->set_line_width(1.0);
	cr->set_source_rgb(0.2,0.2,0.2);
	cr->stroke();
	
	
}

void BookArea::draw_header(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height, Glib::ustring title, int pagen) {
	
	Pango::FontDescription font; 
	
	font.set_family("Sans italic");
	font.set_weight(Pango::WEIGHT_ULTRALIGHT);
	font.set_absolute_size(12*Pango::SCALE);
	
	Glib::RefPtr<Pango::Layout> layout = create_pango_layout("Pride and Prejudice");

	layout->set_font_description(font);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);

	// Position the text in the middle
	cr->move_to((width-text_width)/2, (height * (MARGIN_PERCENT / 100)));

	layout->show_in_cairo_context(cr);
	
	layout = create_pango_layout(to_string(pagen));

	layout->set_font_description(font);

	
	layout->get_pixel_size(text_width, text_height);

	// Position the text in the middle
	
	double position = width - ((width * (MARGIN_PERCENT / 100)) + text_width);
	
	cr->move_to(position, (height * (MARGIN_PERCENT / 100)));

	layout->show_in_cairo_context(cr);


}


void BookArea::draw_text(const Cairo::RefPtr<Cairo::Context>& cr,
                       int rectangle_width, int rectangle_height)
{

    Pango::FontDescription font;

    font.set_family("Serif");
    font.set_weight(Pango::WEIGHT_NORMAL);
    font.set_absolute_size(14*Pango::SCALE);

    Glib::ustring txt = "<span foreground=\"blue\" size=\"x-large\">Blue text</span> is <i>cool</i>! Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam scelerisque sapien luctus lectus placerat porttitor. Proin ac laoreet neque. Aliquam luctus a odio a pharetra. Duis accumsan tellus ac tellus pretium, et interdum magna ornare. Nulla dictum tellus eros, fermentum elementum velit facilisis non. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Nunc et lectus eget mi sollicitudin euismod. Praesent lobortis a sem ultrices accumsan. Cras metus neque, blandit eget semper non, elementum ut diam. In quis ligula et odio tristique hendrerit. Curabitur id ipsum ultricies ligula placerat condimentum quis et massa. Sed enim dolor, vehicula sed feugiat et, hendrerit in nisl. Sed cursus, erat quis venenatis convallis, eros ligula faucibus sem, id pulvinar urna ante non turpis. Cras euismod convallis ipsum quis viverra. Fusce vel urna nisl. Nam scelerisque a nisi sit amet imperdiet. ";

    Glib::ustring ptxt;
    gunichar stuff;

    Pango::AttrList attributes = Pango::AttrList(txt, 0, ptxt, stuff);

    Glib::RefPtr<Pango::Layout> layout = create_pango_layout(ptxt);
    layout->set_attributes(attributes);

    layout->set_font_description(font);

    //layout->set_width((rectangle_width - 20) * Pango::SCALE);
    layout->set_width((rectangle_width * (1 - (2 * (MARGIN_PERCENT/100)))) * Pango::SCALE);

    int text_width;
    int text_height;

    //get the text dimensions (it updates the variables -- by reference)
    layout->get_pixel_size(text_width, text_height);

    cout << "Layout is " << text_width << "x" << text_height << endl;

    layout->set_indent(30 * Pango::SCALE);
    layout->set_spacing(5 * Pango::SCALE);
    layout->set_justify(true);

    int nlines = layout->get_line_count();

    cout << "Layout contains " << nlines << " lines" << endl;

    // Position the text. Horizontal indent, vertical indent. 
    cr->move_to((rectangle_width * (MARGIN_PERCENT / 100)), rectangle_height * (MARGIN_PERCENT * 2 / 100));

    layout->show_in_cairo_context(cr);
    
}

BookReader::BookReader() {
	m_book_area.set_hexpand(true);
	add(m_book_area);
}
	
BookReader::~BookReader() {
	
}






