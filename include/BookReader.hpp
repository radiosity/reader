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

#ifndef BOOKREADER_H
#define BOOKREADER_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/popover.h>
#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/infobar.h>
#include <gtkmm/spinner.h>
#include <string>
#include <vector>

#include <Epub.hpp>

#include "Global.hpp" 
#include "Pages.hpp"

using std::string; 
using std::vector;
using Glib::ustring; 

class BookArea : public Gtk::DrawingArea
{
	public:
		BookArea();
		virtual ~BookArea();
	
		int pagenum;
		Gdk::Rectangle page_num_rect;

	protected:
		//Override default signal handler:
		virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

};

class BookReader : public Gtk::Box {
	
	protected:
		void on_goto_page();
		void on_infobar_response(int);
	
		void info_bar_loading(); 
		void info_bar_hide();
	
	public:
		BookReader();
		~BookReader();

		InfoBar m_infobar;
		Box m_infobar_box; 
		Spinner m_infobar_spinner; 
		Label m_infobar_label;
		
		BookArea m_book_area;
		Popover m_popover;
		Label m_popover_label;
		Entry m_popover_entry; 
		Box m_popover_box;
		Box m_box;

};

#endif // BOOKREADER_H
