#include "BookReader.hpp"

#include <iostream>

using namespace std;

BookArea::BookArea()
{
	cout << "Created" << endl; 
	
	paragraphs.push_back("It is a truth universally acknowledged, that a single man in possession of a good fortune, must be in want of a wife.");
	paragraphs.push_back("However little known the feelings or views of such a man may be on his first entering a neighbourhood, this truth is so well fixed in the minds of the surrounding families, that he is considered the rightful property of some one or other of their daughters.");
	paragraphs.push_back("\"My dear Mr. Bennet,\" said his lady to him one day, \"have you heard that Netherfield Park is let at last?\"");
	paragraphs.push_back("Mr. Bennet replied that he had not.");
	paragraphs.push_back("\"But it is,\" returned she; \"for Mrs. Long has just been here, and she told me all about it.\"");
	paragraphs.push_back("Mr. Bennet made no answer.");
	paragraphs.push_back("\"Do you not want to know who has taken it?\" cried his wife impatiently.");
	paragraphs.push_back("\"You want to tell me, and I have no objection to hearing it.\"");
	paragraphs.push_back("This was invitation enough.");
	paragraphs.push_back("\"Why, my dear, you must know, Mrs. Long says that Netherfield is taken by a young man of large fortune from the north of England; that he came down on Monday in a chaise and four to see the place, and was so much delighted with it, that he agreed with Mr. Morris immediately; that he is to take possession before Michaelmas, and some of his servants are to be in the house by the end of next week.\"");
	paragraphs.push_back("\"What is his name?\"");
	paragraphs.push_back("\"Bingley.\"");
	paragraphs.push_back("\"Is he married or single?\"");
	paragraphs.push_back("\"Oh! Single, my dear, to be sure! A single man of large fortune; four or five thousand a year. What a fine thing for our girls!\"");
	paragraphs.push_back("\"How so? How can it affect them?\"");
	paragraphs.push_back("\"My dear Mr. Bennet,\" replied his wife, \"how can you be so tiresome! You must know that I am thinking of his marrying one of them.\"");
	paragraphs.push_back("\"Is that his design in settling here?\"");
	//paragraphs.push_back("\"Design! Nonsense, how can you talk so! But it is very likely that he may fall in love with one of them, and therefore you must visit him as soon as he comes.\"");
	//paragraphs.push_back("\"I see no occasion for that. You and the girls may go, or you may send them by themselves, which perhaps will be still better, for as you are as handsome as any of them, Mr. Bingley may like you the best of the party.\"");
	//sparagraphs.push_back("\"My dear, you flatter me. I certainly have had my share of beauty, but I do not pretend to be anything extraordinary now. When a woman has five grown-up daughters, she ought to give over thinking of her own beauty.\"");
	//paragraphs.push_back("\"In such cases, a woman has not often much beauty to think of.\"");
	//paragraphs.push_back("\"But, my dear, you must indeed go and see Mr. Bingley when he comes into the neighbourhood.\"");
	//paragraphs.push_back("\"It is more than I engage for, I assure you.\"");
	//paragraphs.push_back("\"But consider your daughters. Only think what an establishment it would be for one of them. Sir William and Lady Lucas are determined to go, merely on that account, for in general, you know, they visit no newcomers. Indeed you must go, for it will be impossible for us to visit him if you do not.\"");
	//paragraphs.push_back("\"You are over-scrupulous, surely. I dare say Mr. Bingley will be very glad to see you; and I will send a few lines by you to assure him of my hearty consent to his marrying whichever he chooses of the girls; though I must throw in a good word for my little Lizzy.\"");
	//paragraphs.push_back("\"I desire you will do no such thing. Lizzy is not a bit better than the others; and I am sure she is not half so handsome as Jane, nor half so good-humoured as Lydia. But you are always giving her the preference.\"");
	
	
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

	cr->set_source_rgb(0.2, 0.2, 0.2);
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
	font.set_variant(Pango::VARIANT_SMALL_CAPS);
	
	Glib::RefPtr<Pango::Layout> layout = create_pango_layout("Pride and Prejudice");

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
                       int rectangle_width, int rectangle_height)
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

		Pango::AttrList attributes = Pango::AttrList("Chapter 1", 0, parsed_txt, stuff);

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
	
	for(string para : paragraphs) {

		Glib::ustring parsed_txt;
		gunichar stuff;

		Pango::AttrList attributes = Pango::AttrList(para, 0, parsed_txt, stuff);

		Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(cr);
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






