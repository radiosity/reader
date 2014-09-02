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

BookArea::BookArea() : pagenum(271)
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
			if (itemid != 0) start_pos += 30;
			start_pos +=DrawingUtils::draw_h2(cr, pci.content, rectangle_width, rectangle_height, start_pos); 
			start_pos += 30;
		}
		else if(pci.pct == PAGE_PARAGRAPH) start_pos += DrawingUtils::draw_text(cr, pci.content, rectangle_width, rectangle_height, start_pos);
		else if(pci.pct == PAGE_FRAGMENT) start_pos += DrawingUtils::draw_fragment(cr, pci.content, rectangle_width, rectangle_height, start_pos);
		
		itemid++; 
		
	}

	return true;
}

BookReader::BookReader() {
	m_book_area.set_hexpand(true);
	add(m_book_area);
}
	
BookReader::~BookReader() {
	
}






