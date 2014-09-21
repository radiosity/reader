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

#ifndef DrawingUtils_H
#define DrawingUtils_H

#include <cairomm/cairomm.h>
#include <pangomm.h>
#include <glibmm.h>
#include <gtkmm.h>
#include <string>
#include <vector>
#include <utility>

using Glib::ustring;

using std::pair;

class DrawingUtils {

	public:

		DrawingUtils() = delete;
		~DrawingUtils() = delete;

		static constexpr double MARGIN_PERCENT = 8.0;
		static constexpr double PHI = 1.618033988749894848204586834365638117720309179805762862135448;

		static void draw_rectangle(const Cairo::RefPtr<Cairo::Context> & cr, int width, int height);
		static Gdk::Rectangle draw_header(const Cairo::RefPtr<Cairo::Context> & cr, const int width, const int height, const ustring title, const int pagen);
		static int draw_h1(const Cairo::RefPtr<Cairo::Context> & cr, const ustring text, const int rectangle_width, const int rectangle_height);
		static bool will_fit_h2(const Cairo::RefPtr<Cairo::Context> & cr, const ustring text, const int rectangle_width, const int rectangle_height, const int start_pos);
		static int draw_h2(const Cairo::RefPtr<Cairo::Context> & cr, const ustring text, const int rectangle_width, const int rectangle_height, const int start_pos);
		static bool will_fit_text(const Cairo::RefPtr<Cairo::Context> & cr, const ustring text, const int rectangle_width, const int rectangle_height, const int start_pos);
		static pair<ustring, ustring> pack_text(const Cairo::RefPtr<Cairo::Context> & cr, const ustring text, const int rectangle_width, const int rectangle_height, const int start_pos);
		static int draw_text(const Cairo::RefPtr<Cairo::Context> & cr, const ustring text, const int rectangle_width, const int rectangle_height, const int start_pos);
		static int draw_fragment(const Cairo::RefPtr<Cairo::Context> & cr, const ustring text, const int rectangle_width, const int rectangle_height, const int start_pos);

};

#endif