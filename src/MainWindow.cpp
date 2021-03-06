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

#include "MainWindow.hpp"
#include "BookReader.hpp"
#include "Global.hpp"

#include <iostream>

using std::cout;
using std::endl;

MainWindow::MainWindow()
{

	set_icon_from_file("icon.png");

	add_events(Gdk::KEY_PRESS_MASK | Gdk::SCROLL_MASK);

	// Window properties
	//set_title("HeaderBar Example");
	set_border_width(0);
	//set_default_size(800,800);

	set_resizable(false);

	// Header bar
	header_bar = &m_header_bar;
	header_bar->set_title("Reader");
	header_bar->set_subtitle("Pride and Prejudice");
	header_bar->set_show_close_button();

	m_create.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_create));
	m_create.set_image_from_icon_name("tab-new-symbolic");
	m_create.set_tooltip_text("Create a new character");
	header_bar->pack_start(m_create);

	m_open_button.set_label("Open");
	m_open_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_open));

	m_open_menu.set_image_from_icon_name("go-down-symbolic");
	m_open_menu.set_tooltip_text("Open a recent file from a list");
	m_open_menu.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_recent));

	Glib::RefPtr< StyleContext > ctx = m_box.get_style_context();
	ctx->add_class(GTK_STYLE_CLASS_LINKED);
	m_box.pack_start(m_open_button);
	m_box.pack_start(m_open_menu);

	header_bar->pack_start(m_box);

	m_save.set_image_from_icon_name("document-save-symbolic");
	m_save.set_tooltip_text("Save the current file");
	m_save.set_sensitive(false);

	header_bar->pack_start(m_save);

	// Set headerbar as titlebar
	set_titlebar(m_header_bar);
	add(m_reader);

	show_all_children();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_destroy()
{
	Main::quit();
}

void MainWindow::on_open()
{
	FileChooserDialog dialog(*this, "Choose a character file", FILE_CHOOSER_ACTION_OPEN);
	//OpenFileDialog dialog("Something", *this, true);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);
	dialog.set_transient_for(*this);
	dialog.run();
}

void MainWindow::on_recent()
{
	RecentChooserDialog dialog(*this, "Choose a character file");
	dialog.run();
}

void MainWindow::on_create()
{

}


bool MainWindow::on_key_press_event(GdkEventKey * event)
{

	#ifdef DEBUG
	cout << "Keyboard Event recieved" << endl;
	#endif

	if(event->keyval == GDK_KEY_Right) {
		#ifdef DEBUG
		cout << "Going forward one page to: " << m_reader.m_book_area.pagenum + 1 << endl;
		#endif

		//Hide the page seek popover, if open:
		m_reader.m_popover.set_visible(false);

		if(pages.is_valid_index(m_reader.m_book_area.pagenum + 1)) {
			m_reader.m_book_area.pagenum++;
			m_reader.m_book_area.queue_draw();
		}

		return true;
	}

	else if(event->keyval == GDK_KEY_Left) {
		#ifdef DEBUG
		cout << "Going backward one page to: " << m_reader.m_book_area.pagenum - 1 << endl;
		#endif

		//Hide the page seek popover, if open:
		m_reader.m_popover.set_visible(false);

		if(m_reader.m_book_area.pagenum != 0) {
			m_reader.m_book_area.pagenum--;
			m_reader.m_book_area.queue_draw();
		}

		return true;
	}

	/*if (event->type == GDK_KEY_PRESS &&
	event->keyval == GDK_KEY_1 &&
	(event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK)*/


	else if(event->keyval == GDK_KEY_g && (event->state & (GDK_CONTROL_MASK)) == GDK_CONTROL_MASK) {

		#ifdef DEBUG
		cout << "Recieved ctrl-g, launching goto popover" << endl;
		#endif

		m_reader.m_popover.set_pointing_to(m_reader.m_book_area.page_num_rect);
		m_reader.m_popover.set_visible(true);

	}

	//if the event has not been handled, call the base class
	return Gtk::Window::on_key_press_event(event);

}

bool MainWindow::on_scroll_event(GdkEventScroll * event)
{

	#ifdef DEBUG
	cout << "Scroll Event recieved" << endl;
	#endif

	//Hide the page seek popover, if open:
	m_reader.m_popover.set_visible(false);

	if(event->direction == GDK_SCROLL_DOWN) {
		#ifdef DEBUG
		cout << "Going forward one page to: " << m_reader.m_book_area.pagenum + 1 << endl;
		#endif

		if(pages.is_valid_index(m_reader.m_book_area.pagenum + 1)) {
			m_reader.m_book_area.pagenum++;
			m_reader.m_book_area.queue_draw();
		}

		return true;
	}

	else if(event->direction == GDK_SCROLL_UP) {
		#ifdef DEBUG
		cout << "Going backward one page to: " << m_reader.m_book_area.pagenum - 1 << endl;
		#endif

		if(m_reader.m_book_area.pagenum != 0) {
			m_reader.m_book_area.pagenum--;
			m_reader.m_book_area.queue_draw();
		}

		return true;
	}

	return Gtk::Window::on_scroll_event(event);

}

/*
void MainWindow::on_switch_active_changed()
{
	bool show_close_button = m_switch.get_active();
	header_bar->set_show_close_button(show_close_button);
}

void MainWindow::on_title_entry_activate()
{
	const Glib::ustring title = m_title_entry.get_text();
	if(!title.empty())
	{
	header_bar->set_title(title);
	}
}

void MainWindow::on_subtitle_entry_activate()
{
	const Glib::ustring subtitle = m_subtitle_entry.get_text();
	header_bar->set_subtitle(subtitle);
}

void MainWindow::on_button_clicked()
{
	Gtk::MessageDialog dialog(*this, "Button clicked", true);
	dialog.run();
}
*/
