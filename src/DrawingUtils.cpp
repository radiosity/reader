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

#include "DrawingUtils.hpp"

#include <string>
#include <list>
#include <utility>
#include <iostream>
#include <pangomm/init.h>

using namespace std; 

namespace {
	
	inline Pango::FontDescription __get_font(string family, Pango::Weight weight, int size) {
		
		Pango::FontDescription font; 
	
		font.set_family(family);
		font.set_weight(weight);
		font.set_absolute_size(size*Pango::SCALE);
		
		return font; 
		
	}
	
}

void DrawingUtils::draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr,
                            const int width, const int height)
{
	
	cr->rectangle(0, 0, width, height);
	cr->fill();
	cr->rectangle(0, 0, width, height);
	cr->set_line_width(1.0);
	cr->set_source_rgb(0.2,0.2,0.2);
	cr->stroke();
	
}

void DrawingUtils::draw_header(const Cairo::RefPtr<Cairo::Context>& cr, const int width, const int height, const ustring title, const int pagen) {
	
	Pango::FontDescription font = __get_font("EB Garamond", Pango::WEIGHT_ULTRALIGHT, 12); 
	font.set_variant(Pango::VARIANT_SMALL_CAPS);
	
	ustring parsed_txt;
	gunichar stuff;

	auto attributes = Pango::AttrList(title, 0, parsed_txt, stuff);

	Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(cr);
	layout->set_attributes(attributes);
	layout->set_font_description(font);
	layout->set_text(parsed_txt);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);

	// Position the text in the middle
	cr->move_to((width-text_width)/2, (height * (MARGIN_PERCENT / 100))/2);

	layout->show_in_cairo_context(cr);
	
	attributes = Pango::AttrList(to_string(pagen), 0, parsed_txt, stuff);

	font.set_absolute_size(16*Pango::SCALE);

	layout = Pango::Layout::create(cr);
	layout->set_attributes(attributes);
	layout->set_font_description(font);
	layout->set_text(parsed_txt);

	layout->get_pixel_size(text_width, text_height);
	
	double position = width - ((width * (MARGIN_PERCENT / 100)) + text_width);
	
	cr->move_to(position, height - (height * (MARGIN_PERCENT / 200)) - text_height);

	layout->show_in_cairo_context(cr);


}

int DrawingUtils::draw_h1(const Cairo::RefPtr<Cairo::Context>& cr, const ustring text,
                       const int rectangle_width, const int rectangle_height)
{
	
	Pango::FontDescription font = __get_font("EB Garamond", Pango::WEIGHT_NORMAL, 72); 
	
	ustring parsed_txt;
	gunichar stuff;

	auto attributes = Pango::AttrList(text, 0, parsed_txt, stuff);

	Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(cr);
	layout->set_attributes(attributes);
	layout->set_text(parsed_txt);
	layout->set_font_description(font);
	layout->set_alignment(Pango::Alignment::ALIGN_CENTER);
	layout->set_width((rectangle_width * (1 - (2 * (MARGIN_PERCENT/100)))) * Pango::SCALE);
	
	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);

	cr->move_to(rectangle_width * (MARGIN_PERCENT/100), ((rectangle_height)/(1+PHI))-(text_height/2));

	layout->show_in_cairo_context(cr);
	
	return ((rectangle_height - text_height)/(PHI*2)) + text_height;		

}

namespace {
	
	Glib::RefPtr<Pango::Layout> __get_h2_layout(const Cairo::RefPtr<Cairo::Context>& cr, 
										const ustring text,
										const int rectangle_width, 
										const int rectangle_height) 
		{
		
			Pango::FontDescription font = __get_font("EB Garamond", Pango::WEIGHT_NORMAL, 40); 
	
			ustring parsed_txt;
			gunichar stuff;

			auto attributes = Pango::AttrList(text, 0, parsed_txt, stuff);

			Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(cr);
			layout->set_attributes(attributes);
			layout->set_text(parsed_txt);
			layout->set_font_description(font);
			layout->set_alignment(Pango::Alignment::ALIGN_CENTER);
			layout->set_width((rectangle_width * (1 - (2 * (DrawingUtils::MARGIN_PERCENT/100)))) * Pango::SCALE);
			
			return layout; 
			
		}
	
}

bool DrawingUtils::will_fit_h2(const Cairo::RefPtr<Cairo::Context>& cr, const ustring text,
                       const int rectangle_width, const int rectangle_height, const int start_pos)
{
	
	Glib::RefPtr<Pango::Layout> layout = __get_h2_layout(cr, text, rectangle_width, rectangle_height);
	
	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);
	
	int val = start_pos + text_height; 
	int threshold = rectangle_height * (1 - (2 * (MARGIN_PERCENT/100)));

	if(val <= threshold) return true; 
	else return false; 	

}

int DrawingUtils::draw_h2(const Cairo::RefPtr<Cairo::Context>& cr, const ustring text,
                       const int rectangle_width, const int rectangle_height, const int start_pos)
{
	
	Glib::RefPtr<Pango::Layout> layout = __get_h2_layout(cr, text, rectangle_width, rectangle_height);
	
	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);

	cr->move_to(rectangle_width * (MARGIN_PERCENT/100), start_pos + rectangle_height * (MARGIN_PERCENT * 2 / 100)/2);

	layout->show_in_cairo_context(cr);
	
	return text_height; 	

}

namespace {
	
	Glib::RefPtr<Pango::Layout> __get_text_layout(const Cairo::RefPtr<Cairo::Context>& cr, 
										const ustring text,
										const int rectangle_width, 
										const int rectangle_height) 
		{
		
			Pango::FontDescription font = __get_font("EB Garamond", Pango::WEIGHT_NORMAL, 16); 
		
			ustring parsed_txt;
			gunichar stuff;

			auto attributes = Pango::AttrList(text, 0, parsed_txt, stuff);

			auto layout = Pango::Layout::create(cr);
			layout->set_attributes(attributes);
			layout->set_text(parsed_txt);
			layout->set_font_description(font);
			layout->set_width((rectangle_width * (1 - (2 * (DrawingUtils::MARGIN_PERCENT/100)))) * Pango::SCALE);
			layout->set_indent(30 * Pango::SCALE);
			layout->set_justify(true);
					
			return layout; 
			
		}
	
}

bool DrawingUtils::will_fit_text(const Cairo::RefPtr<Cairo::Context>& cr, const ustring text,
                       const int rectangle_width, const int rectangle_height, const int start_pos)
{
	
	Glib::RefPtr<Pango::Layout> layout = __get_text_layout(cr, text, rectangle_width, rectangle_height);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);
	
	int val = start_pos + text_height; 
	int threshold = rectangle_height * (1 - (2 * (MARGIN_PERCENT/100)));

	if(val <= threshold) return true; 
	else return false; 

}

class Word {
	
	public:
		
	const int index; 
	list<ustring> open_tags; 
	
	Word(const int _i) : index(_i), open_tags() { } 
	~Word() { }
	
};



class UstringSplitter2 {
	
	/*
	
	Need to write this down. Inputs. 
	
	Hello this is a <i>text entry</i> sentence.
	Hello
	Hello this
	Hello this is 
	Hello this is a 
	Hello this is a <i>text</i>
	
	*/
	
	private:
		const ustring content; 
		vector<Word> words; 

	public: 
		UstringSplitter2(ustring _content) : content(_content) {

			list<ustring> open_tags;
			
			unsigned int i = 0;
			unsigned int content_size = content.size();
			
			bool expecting_word_end = true; 
			
			//fix range check here
			while(i < content_size) {
				
				const gunichar start = content[i];
				i++; 
				
				if(start == '<'){
				
					const gunichar next = content[i]; 
					
					if(next == '/') {
						const ustring tag = open_tags.front();
						open_tags.pop_front();
						
						i = content.find('>', i) + 1;
					}
					else {
						//opening a tag.
						int tmp = content.find(">", i);
						
						const ustring tag = content.substr(i, (tmp-i));
						open_tags.push_front(tag);
						
						i = tmp + 1; 
					}
					
				}
				else if(g_unichar_isspace(start)) {
					//
					if(expecting_word_end) {
						Word tmp(i); 
						tmp.open_tags = open_tags; 
						words.push_back(tmp);
					}
					
				}
				else {
					
					expecting_word_end = true; 
					
				}	
				
			}
			
			if(expecting_word_end) {
				Word tmp(i); 
				tmp.open_tags = open_tags; 
				words.push_back(tmp);
			}


		}
		~UstringSplitter2() { }
		
		unsigned int nwords() {
			
			return words.size();
			
		}
		
		ustring get(const unsigned int word_index) {
			
			Word w = words[word_index];
			
			ustring tmp = content.substr(0, w.index);
			
			for(const ustring tag : w.open_tags) {
				tmp += "</";
				tmp += tag;
				tmp += '>';
			}
			
			return tmp; 
			
		}
		
		ustring get_fragment(unsigned int word_index) {
			
			Word w = words[word_index];
			
			ustring tmp = "";
			
			for(const ustring tag : w.open_tags) {
				tmp += "<";
				tmp += tag;
				tmp += '>';
			}
			
			tmp += content.substr(w.index, content.size());
			
			return tmp; 
			
		}
	
};

pair<ustring, ustring> DrawingUtils::pack_text(const Cairo::RefPtr<Cairo::Context>& cr, const ustring text,
                       const int rectangle_width, const int rectangle_height, const int start_pos)
{
	
	UstringSplitter2 tmp(text);
	unsigned int nwords = tmp.nwords();

	ustring working_text = "";
	
	//Special case. If there's only one word, and we're calling pack text, 
	//it clearly can't be packed. 
	
	if(nwords == 1) { 
		return pair<ustring, ustring>(working_text, text); 
	}
	
	for(unsigned int i = 0; i < nwords; i++) {
	
		ustring clipped_text = tmp.get(i+1);
		
		if(will_fit_text(cr, clipped_text, rectangle_width, rectangle_height, start_pos)) {
			//it fits;
			working_text = clipped_text; 
		}
		else {
			//doesn't fit.
			if(i == 0) {
				//special case. Nothing fits, we can't pack anything. Return the whole thing. 
				return pair<ustring, ustring>(working_text, text); 
				
			}
			else {
				//Draw it
				draw_text(cr, working_text, rectangle_width, rectangle_height, start_pos); 
				
				//finally, return the fragment.
				return pair<ustring, ustring>(working_text, tmp.get_fragment(i));
			}
		}
	
	}
	
	//shouldn't reach here
	return pair<ustring, ustring>("", ""); 
	
}

int DrawingUtils::draw_text(const Cairo::RefPtr<Cairo::Context>& cr, const ustring text,
                       const int rectangle_width, const int rectangle_height, const int start_pos)
{
	
	Glib::RefPtr<Pango::Layout> layout = __get_text_layout(cr, text, rectangle_width, rectangle_height);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);

	cr->move_to((rectangle_width * (MARGIN_PERCENT / 100)), start_pos + rectangle_height * (MARGIN_PERCENT * 2 / 100)/2);

	layout->show_in_cairo_context(cr);
	
	return text_height;

}

int DrawingUtils::draw_fragment(const Cairo::RefPtr<Cairo::Context>& cr, const ustring text,
                       const int rectangle_width, const int rectangle_height, const int start_pos)
{
	
	Pango::FontDescription font = __get_font("EB Garamond", Pango::WEIGHT_NORMAL, 16); 
		
	ustring parsed_txt;
	gunichar stuff;

	auto attributes = Pango::AttrList(text, 0, parsed_txt, stuff);

	auto layout = Pango::Layout::create(cr);
	layout->set_attributes(attributes);
	layout->set_text(parsed_txt);
	layout->set_font_description(font);
	layout->set_width((rectangle_width * (1 - (2 * (MARGIN_PERCENT/100)))) * Pango::SCALE);
	layout->set_justify(true);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);

	cr->move_to((rectangle_width * (MARGIN_PERCENT / 100)), start_pos + rectangle_height * (MARGIN_PERCENT * 2 / 100)/2);

	layout->show_in_cairo_context(cr);
	
	return text_height;

}