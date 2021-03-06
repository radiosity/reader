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

	//set_border_width(15);

	set_size_request(600, 900);

	#ifdef DEBUG
	cout << "Created" << endl;
	cout << "Loading book" << endl;
	#endif

}

BookArea::~BookArea()
{

}

bool BookArea::on_draw(const Cairo::RefPtr<Cairo::Context> & cr)
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

	cr->set_source_rgb(0.15, 0.15, 0.15);
	//cr->set_source_rgb(1, 0.33, 0.33);

	int start_pos = 0;

	int itemid = 0;

	for(PageContentItem pci : pd.items) {

		if(pci.pct == PAGE_H1) {
			start_pos += DrawingUtils::draw_h1(cr, pci.content, rectangle_width, rectangle_height);
		}
		else if(pci.pct == PAGE_H2) {
			// Only pack out the header when it's not the first thing in the page
			if (itemid != 0) {
				start_pos += 35;
			}

			start_pos += DrawingUtils::draw_h2(cr, pci.content, rectangle_width, rectangle_height, start_pos);
			start_pos += 35;
		}
		else if(pci.pct == PAGE_PARAGRAPH) {
			start_pos += DrawingUtils::draw_text(cr, pci.content, rectangle_width, rectangle_height, start_pos);
		}
		else if(pci.pct == PAGE_FRAGMENT) {
			start_pos += DrawingUtils::draw_fragment(cr, pci.content, rectangle_width, rectangle_height, start_pos);
		}

		itemid++;

	}
	
	//auto it = book.opf_files[0].metadata.find(MetadataType::TITLE);
	//const ustring title = it->second.contents;
	cr->set_source_rgb(0.5, 0.5, 0.5);
	page_num_rect = DrawingUtils::draw_header(cr, rectangle_width, rectangle_height, "Pride and Prejudice", pagenum + 1);
	
	return true;
}

BookReader::BookReader() :
	m_popover(m_book_area),
	m_popover_label("Go to page:"),
	m_popover_box(Gtk::ORIENTATION_HORIZONTAL, 12)
{

	//sort out signal handlers.
	info_bar_hide_dispatcher.connect(sigc::mem_fun(*this, &BookReader::info_bar_hide));

	set_orientation(ORIENTATION_VERTICAL);


	m_infobar_label.set_text("");
	m_infobar_box.set_orientation(ORIENTATION_HORIZONTAL);
	m_infobar_box.set_spacing(12);
	m_infobar_box.pack_start(m_infobar_spinner);
	m_infobar_box.pack_start(m_infobar_label);
	m_infobar_box.show_all();
	Gtk::Container * infoBarContainer =
	    dynamic_cast<Gtk::Container *>(m_infobar.get_content_area());

	if (infoBarContainer)	{
		infoBarContainer->add(m_infobar_box);
	}

	m_infobar.set_no_show_all(true);
	m_infobar.set_show_close_button(true);
	m_infobar.signal_response().connect(sigc::mem_fun( *this, &BookReader::on_infobar_response) );

	pack_start(m_infobar, Gtk::PACK_SHRINK);

	m_book_area.set_hexpand(true);
	m_box.pack_start(m_book_area);
	m_box.set_border_width(15);

	pack_start(m_box);

	//m_entry.set_max_length(10);
	m_popover_entry.set_width_chars(4);
	m_popover_entry.set_max_width_chars(4);
	m_popover_entry.set_text("");
	m_popover_entry.signal_activate().connect(sigc::mem_fun(*this, &BookReader::on_goto_page));

	m_popover_box.set_vexpand(true);
	m_popover_box.set_hexpand(true);
	m_popover_box.pack_start(m_popover_label);
	m_popover_box.pack_start(m_popover_entry);
	m_popover_box.show_all();

	//m_popover.add_label("Sup holmes");
	m_popover.add(m_popover_box);
	m_popover.set_position(Gtk::POS_LEFT);
	m_popover.set_border_width(8);
	m_popover.set_modal(true);
	//m_popover.set_visible(true);

	auto stylecontext = m_popover.get_style_context();
	stylecontext->add_class(GTK_STYLE_CLASS_INFO);

	if(!pages.finished_loading()) {
		info_bar_loading();
	}
	else {
		info_bar_hide();
	}

}

BookReader::~BookReader()
{

}

void BookReader::on_infobar_response(int)
{
	info_bar_hide();
}

void BookReader::info_bar_loading()
{
	m_infobar_label.set_text("Importing new book into Library, this may take time...");
	m_infobar_spinner.start();
	m_infobar.show();
}

void BookReader::info_bar_hide()
{
	m_infobar_label.set_text("");
	m_infobar_spinner.stop();
	m_infobar.hide();
}

void BookReader::on_goto_page()
{

	#ifdef DEBUG
	cout << "Updating page via popover callback" << endl;
	#endif

	ustring r = m_popover_entry.get_text();

	m_popover_entry.set_text("");
	m_popover.set_visible(false);

	try {
		unsigned int page = std::stoi(r.raw());
		m_book_area.pagenum = page - 1;
		m_book_area.queue_draw();
	}

	catch(std::invalid_argument e) {
		#ifdef DEBUG
		cout << "Could not convert, text was " << r << endl;
		#endif
	}

}



