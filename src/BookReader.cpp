/*
Copyright (c) 2014, Richard Martin
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Richard Martin nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL RICHARD MARTIN BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "BookReader.hpp"

#include <iostream>

using namespace std;

BookArea::BookArea() : book("book")
{
	cout << "Created" << endl; 
	
	cout << "Loading book" << endl; 
	
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

	auto it = book.opf_files[0].metadata.find(MetadataType::TITLE);
	const ustring title = it->second.contents;
	cr->set_source_rgb(0.5, 0.5, 0.5);	
	draw_header(cr, rectangle_width, rectangle_height, title, 21);

	cr->set_source_rgb(0.2, 0.2, 0.2);
	draw_text(cr, rectangle_width, rectangle_height);

	return true;
}

void BookArea::draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr,
                            const int width, const int height)
{
	
	cr->rectangle(0, 0, width, height);
	cr->fill();
	cr->rectangle(0, 0, width, height);
	cr->set_line_width(1.0);
	cr->set_source_rgb(0.2,0.2,0.2);
	cr->stroke();
	
	
}

void BookArea::draw_header(const Cairo::RefPtr<Cairo::Context>& cr, const int width, const int height, const Glib::ustring title, const int pagen) {
	
	Pango::FontDescription font; 
	
	font.set_family("Sans italic");
	font.set_weight(Pango::WEIGHT_ULTRALIGHT);
	font.set_absolute_size(12*Pango::SCALE);
	font.set_variant(Pango::VARIANT_SMALL_CAPS);
	
	Glib::RefPtr<Pango::Layout> layout = create_pango_layout(title);

	layout->set_font_description(font);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);

	// Position the text in the middle
	cr->move_to((width-text_width)/2, (height * (MARGIN_PERCENT / 100))/2);

	layout->show_in_cairo_context(cr);
	
	layout = create_pango_layout(to_string(pagen));

	layout->set_font_description(font);

	
	layout->get_pixel_size(text_width, text_height);
	
	double position = width - ((width * (MARGIN_PERCENT / 100)) + text_width);
	
	cr->move_to(position, (height * (MARGIN_PERCENT / 100))/2);

	layout->show_in_cairo_context(cr);


}


void BookArea::draw_text(const Cairo::RefPtr<Cairo::Context>& cr,
                       const int rectangle_width, const int rectangle_height)
{

	
	//Draw a chapter title bit thing. 
	
	Pango::FontDescription font;

	font.set_family("Gentium Book Basic");
	font.set_weight(Pango::WEIGHT_NORMAL);
	font.set_absolute_size(32*Pango::SCALE);
	
	unsigned int start_pos = 0;
	
	{
	
		Glib::ustring parsed_txt;
		gunichar stuff;

		auto attributes = Pango::AttrList("Chapter 1", 0, parsed_txt, stuff);

		Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(cr);
		layout->set_attributes(attributes);
		layout->set_text(parsed_txt);
		layout->set_font_description(font);
		
		int text_width;
		int text_height;
	
		layout->get_pixel_size(text_width, text_height);
	
		cr->move_to((rectangle_width-text_width)/2, (rectangle_height * (MARGIN_PERCENT / 100)) * 1.5);
	
		layout->show_in_cairo_context(cr);
		
		start_pos += (rectangle_height * (MARGIN_PERCENT / 100)) * 2;
		
	}
	
	font.set_absolute_size(16*Pango::SCALE);
	
	bool first = true;
	
	for(const ContentItem c : book.contents[0].items) {

		Glib::ustring parsed_txt;
		gunichar stuff;

		auto attributes = Pango::AttrList(c.content, 0, parsed_txt, stuff);

		auto layout = Pango::Layout::create(cr);
		layout->set_attributes(attributes);
		layout->set_text(parsed_txt);
		layout->set_font_description(font);
		layout->set_width((rectangle_width * (1 - (2 * (MARGIN_PERCENT/100)))) * Pango::SCALE);
		if(first){
			first = false; 
		}
		else {
			layout->set_indent(30 * Pango::SCALE);
		}
			
		//layout->set_spacing(5 * Pango::SCALE);
		layout->set_justify(true);

		int text_width;
		int text_height;

		//get the text dimensions (it updates the variables -- by reference)
		layout->get_pixel_size(text_width, text_height);

		cout << "Layout is " << text_width << "x" << text_height << endl;

		int nlines = layout->get_line_count();

		cout << "Layout contains " << nlines << " lines" << endl;

		// Position the text. Horizontal indent, vertical indent. 
		cr->move_to((rectangle_width * (MARGIN_PERCENT / 100)), start_pos + rectangle_height * (MARGIN_PERCENT * 2 / 100)/2);

		layout->show_in_cairo_context(cr);
		
		start_pos += text_height;
	
	}

}

BookReader::BookReader() {
	m_book_area.set_hexpand(true);
	add(m_book_area);
}
	
BookReader::~BookReader() {
	
}






