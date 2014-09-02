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

#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#include <gtkmm.h>

#include "BookReader.hpp"

using namespace Gtk; 

class MainWindow : public Window
{

public:
	MainWindow();
	virtual ~MainWindow();

protected:
	// Slots
	/*void on_switch_active_changed();
	void on_title_entry_activate();
	void on_subtitle_entry_activate();
	void on_button_clicked();
	void on_close();*/ 

	void on_open();
	void on_recent();
	void on_destroy();
	void on_create();

	//Book Reader
	BookReader m_reader; 

	//Header Bar
	HeaderBar m_header_bar;
	Box m_box; 
	Button m_create; 
	Button m_open_button; 
	Button m_open_menu; 
	Button m_save; 
	
	//Signal handler
	virtual bool on_key_press_event(GdkEventKey * event);
	virtual bool on_scroll_event(GdkEventScroll * event);


private:
};

#endif