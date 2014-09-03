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
#include <gtkmm.h>

#include "DrawingUtils.hpp"

using namespace std;

BookArea::BookArea() : pagenum(0)
{
	#ifdef DEBUG
	cout << "Created" << endl; 
	cout << "Loading book" << endl; 
	#endif
	
}

BookArea::~BookArea()
{
	
}

bool BookArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{

	Cairo::RefPtr<Cairo::Surface> surface = cr->get_target();
	
	PageDescriptor pd = pages.get(pagenum);

	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
		
	const int rectangle_width = width;
	const int rectangle_height = height;

	//cr->set_antialias(ANTIALIAS_BEST);

	cr->set_source_rgb(1.0, 1.0, 1.0);
	DrawingUtils::draw_rectangle(cr, rectangle_width, rectangle_height);

	//auto it = book.opf_files[0].metadata.find(MetadataType::TITLE);
	//const ustring title = it->second.contents;
	cr->set_source_rgb(0.5, 0.5, 0.5);	
	DrawingUtils::draw_header(cr, rectangle_width, rectangle_height, "Pride and Prejudice", pagenum + 1);
	
	cr->set_source_rgb(0.15, 0.15, 0.15);
	//cr->set_source_rgb(1, 0.33, 0.33);
	
	int start_pos = 0; 
	
	int itemid = 0; 
	
	for(PageContentItem pci : pd.items) {
		
		if(pci.pct == PAGE_H1) start_pos += DrawingUtils::draw_h1(cr, pci.content, rectangle_width, rectangle_height); 
		else if(pci.pct == PAGE_H2) {
			// Only pack out the header when it's not the first thing in the page
			if (itemid != 0) start_pos += 35;
			start_pos +=DrawingUtils::draw_h2(cr, pci.content, rectangle_width, rectangle_height, start_pos); 
			start_pos += 35;
		}
		else if(pci.pct == PAGE_PARAGRAPH) start_pos += DrawingUtils::draw_text(cr, pci.content, rectangle_width, rectangle_height, start_pos);
		else if(pci.pct == PAGE_FRAGMENT) start_pos += DrawingUtils::draw_fragment(cr, pci.content, rectangle_width, rectangle_height, start_pos);
		
		itemid++; 
		
	}
	
	return true;
}

BookReader::BookReader() : m_popover(m_book_area), m_popover_label("Go to page...") {
	m_book_area.set_hexpand(true);
	add(m_book_area);
	
	//m_entry.set_max_length(10);
	m_popover_entry.set_width_chars(4);
	m_popover_entry.set_max_width_chars(4);
	m_popover_entry.set_text("");
	m_popover_entry.signal_activate().connect(sigc::mem_fun(*this, &BookReader::on_goto_page));
	
	m_popover_grid.set_row_spacing(6);
	m_popover_grid.set_column_spacing(6);
	m_popover_grid.attach(m_popover_label, 1, 1, 1, 1);
	m_popover_grid.attach(m_popover_entry, 1, 2, 1, 1);
	m_popover_grid.show_all();
	
	Gdk::Rectangle rect;
	rect.set_x(550);
	rect.set_y(850);
	rect.set_width(1);
	rect.set_height(1);
	
	m_popover.set_pointing_to(rect);
	m_popover.add(m_popover_grid);
	m_popover.set_position(Gtk::POS_TOP);
	m_popover.set_border_width(6);
	m_popover.set_modal(true);
	//m_popover.set_visible(true);
	
}
	
BookReader::~BookReader() {
	
}

void BookReader::on_goto_page() {
	
	cout << "success" << endl; 
	
	ustring r = m_popover_entry.get_text();
	unsigned int page = std::stoi(r.raw());
	m_book_area.pagenum = page - 1; 
	m_book_area.queue_draw(); 
	
	
	m_popover_entry.set_text("");
	
	m_popover.set_visible(false);
	
}





