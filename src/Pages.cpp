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

#include "Pages.hpp"

#include <string>
#include <list>
#include <utility>
#include <iostream>
#include <pangomm/init.h>
#include <boost/filesystem.hpp>
#include <thread>
#include <sqlite3.h> 

#include "DrawingUtils.hpp"

using namespace std;
using namespace boost::filesystem;

///// PageContentItems first

PageContentItem::PageContentItem(PageContentType _pct, ustring _content) :
	pct(_pct), 
	content(_content)
{
	
}
		
PageContentItem::PageContentItem(PageContentItem const & cpy) :
	pct(cpy.pct), 
	content(cpy.content)
{
	
}

PageContentItem::PageContentItem(PageContentItem && mv)  :
	pct(move(mv.pct)), 
	content(move(mv.content))
{
	
}

PageContentItem& PageContentItem::operator =(const PageContentItem& cpy) {
	pct = cpy.pct; 
	content = cpy.content; 
	return *this; 
}

PageContentItem& PageContentItem::operator =(PageContentItem && mv)  {
	pct = move(mv.pct); 
	content = move(mv.content); 
	return *this;
}
	
PageContentItem::~PageContentItem() {
	
}

// Page Descriptor next. 
	
PageDescriptor::PageDescriptor() : items()
{
	
}

PageDescriptor::PageDescriptor::PageDescriptor(PageDescriptor const & cpy) :
	items(cpy.items) 
{
	
}

PageDescriptor::PageDescriptor(PageDescriptor && mv) :
	items(move(mv.items))
{
	
}

PageDescriptor& PageDescriptor::operator =(const PageDescriptor& cpy) {
	items = cpy.items; 
	return *this;
}

PageDescriptor& PageDescriptor::operator =(PageDescriptor && mv) {
	items = move(mv.items); 
	return *this; 
}
	
PageDescriptor::~PageDescriptor() {

}

// Pages

Pages::Pages() : 
	mtx(),
	ready(),
	descriptors(),
	finished(false)
{
	
}
	
Pages::~Pages() {

} 	

bool Pages::available(const unsigned int n) const {
	return n < descriptors.size();
}

PageDescriptor Pages::get(const unsigned int n) {

	unique_lock<mutex> lck(mtx); 
	
	if(available(n)) {
		//we have it
		return descriptors[n];
	}
	else {
		//we don't have it
		while(!available(n)) ready.wait(lck);
		return descriptors[n];
	}
	
}

void Pages::add(PageDescriptor p) {

	unique_lock<mutex> lck(mtx); 
	
	descriptors.push_back(p); 
	ready.notify_one(); 
	
}

// Now the Paginator iteself

//anonymous namespace for epub loading and
//sqlite loading helper functions
namespace {
	
	void __load_epub(Pages &pages, path filename, path dbfilename) {
		
		cout << "Loading Epub" << endl; 
		
		//So, first let's sort out the database 
		
		sqlite3 * db; 
		int rc;
		char* errmsg;

		rc = sqlite3_open(dbfilename.c_str(), &db);

		if( rc ){
			//Failed to open
		}
		
		const string table_sql = "CREATE TABLE book("  \
							"pagen INT NOT NULL," \
							"descriptorid INT NOT NULL," \
							"contenttype INT NOT NULL," \
							"content TEXT NOT NULL ) ;";
		
		sqlite3_stmt * tmpstmt;
		
		rc = sqlite3_prepare_v2(db, table_sql.c_str(), -1, &tmpstmt, 0);
		if(rc != SQLITE_OK && rc != SQLITE_DONE) throw -1;
		rc = sqlite3_step(tmpstmt);
		if(rc != SQLITE_OK && rc != SQLITE_DONE) throw -1;
		sqlite3_finalize(tmpstmt);
		
		//Table created. 
		
		const string book_insert_sql = "INSERT INTO book (pagen, descriptorid, contenttype, content) VALUES (?, ?, ?, ?);";
		sqlite3_stmt * book_insert; 
		
		rc = sqlite3_prepare_v2(db, book_insert_sql.c_str(), -1, &book_insert, 0);
		if(rc != SQLITE_OK && rc != SQLITE_DONE) throw -1;
		
		//Do all the following inserts in an SQLite Transaction, because this speeds up the inserts like crazy. 
		sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errmsg);
		
		//Unpack the ebook
		Epub book(filename.c_str());
		
		//Initialise Pango for the journey ahead. 
		Pango::init();
		
		unsigned int epub_content_index = 0; 
		unsigned int epub_content_length = book.contents[0].items.size();
		
		bool has_fragment = false; 
		ustring fragment = "";
		
		unsigned int pagenum = 0; 
		unsigned int itemid = 0; 
		
		for( ; epub_content_index < epub_content_length ; pagenum++) {
				
			//Create a new PageDescriptor to store the items we're going to create. 
			PageDescriptor pd;
			
			cout << "Processing page " << pagenum << endl; 
			
			//reset the id counter
			itemid = 0; 
			
			string filename = "pages/page";
			filename += to_string(pagenum+1); 
			filename += ".pdf"; 
		
			const int width = 600;
			const int height = 900;
			
			Cairo::RefPtr<Cairo::PdfSurface> surface =
				Cairo::PdfSurface::create(filename, width, height);
			Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);
			
			const int rectangle_width = width;
			const int rectangle_height = height;

			//cr->set_antialias(ANTIALIAS_BEST);

			cr->set_source_rgb(1.0, 1.0, 1.0);
			DrawingUtils::draw_rectangle(cr, rectangle_width, rectangle_height);

			auto it = book.opf_files[0].metadata.find(MetadataType::TITLE);
			const ustring title = it->second.contents;
			cr->set_source_rgb(0.5, 0.5, 0.5);	
			DrawingUtils::draw_header(cr, rectangle_width, rectangle_height, title, pagenum);
			
			it = book.opf_files[0].metadata.find(MetadataType::CREATOR);
			const ustring author = it->second.contents;

			int start_pos = 0; 

			cr->set_source_rgb(0.1, 0.1, 0.1);

			if(has_fragment) {
				
				sqlite3_bind_int(book_insert, 1, pagenum);
				sqlite3_bind_int(book_insert, 2, itemid);
				sqlite3_bind_int(book_insert, 3, PAGE_FRAGMENT);
				sqlite3_bind_text(book_insert, 4, fragment.c_str(), -1, SQLITE_STATIC);

				int result = sqlite3_step(book_insert);
				if(result != SQLITE_OK && result != SQLITE_ROW && result != SQLITE_DONE) throw -1;

				sqlite3_reset(book_insert);
				
				pd.items.push_back(PageContentItem(PAGE_FRAGMENT, fragment));
				
				has_fragment = false; 
				start_pos += DrawingUtils::draw_fragment(cr, fragment, rectangle_width, rectangle_height, start_pos); 
				fragment = "";
				
				itemid++;
			}
			
			while(epub_content_index < epub_content_length) {
				
				sqlite3_bind_int(book_insert, 1, pagenum);
				sqlite3_bind_int(book_insert, 2, itemid);
				
				const ContentItem c = book.contents[0].items[epub_content_index]; 
				epub_content_index++; 
				
				if(c.type == H1) {
					sqlite3_bind_int(book_insert, 3, PAGE_H1);
					sqlite3_bind_text(book_insert, 4, c.content.c_str(), -1, SQLITE_STATIC);	
					
					pd.items.push_back(PageContentItem(PAGE_H1, c.content));
					
					start_pos += DrawingUtils::draw_h1(cr, c.content, rectangle_width, rectangle_height); 
				}
				else if (c.type == H2) {
					sqlite3_bind_int(book_insert, 3, PAGE_H2);
					sqlite3_bind_text(book_insert, 4, c.content.c_str(), -1, SQLITE_STATIC);
					
					pd.items.push_back(PageContentItem(PAGE_H2, c.content));
					
					start_pos += 30;
					start_pos += DrawingUtils::draw_h2(cr, c.content, rectangle_width, rectangle_height, start_pos); 
					start_pos += 30; 
				}	
				else if (c.type == P) {
					//cout << c.content << endl; 
					if(DrawingUtils::will_fit_text(cr, c.content, rectangle_width, rectangle_height, start_pos)) {
						sqlite3_bind_int(book_insert, 3, PAGE_PARAGRAPH);
						sqlite3_bind_text(book_insert, 4, c.content.c_str(), -1, SQLITE_STATIC);
						
						pd.items.push_back(PageContentItem(PAGE_PARAGRAPH, c.content));
						
						start_pos += DrawingUtils::draw_text(cr, c.content, rectangle_width, rectangle_height, start_pos);
					}
					else {
						pair<ustring, ustring> packres = DrawingUtils::pack_text(cr, c.content, rectangle_width, rectangle_height, start_pos);
						fragment = packres.second; 
						if(fragment.compare(c.content) == 0) {
							//can't pack the content. Go back one, and set a new page, so we do all this again. 
							epub_content_index--; 
						}
						else {
							has_fragment = true; 
							
							pd.items.push_back(PageContentItem(PAGE_PARAGRAPH, packres.first));
							
							sqlite3_bind_int(book_insert, 3, PAGE_PARAGRAPH);
							sqlite3_bind_text(book_insert, 4, packres.first.c_str(), -1, SQLITE_STATIC);
						}
						break;
					}
				}
				else if (c.type == HR) {
					break; 
				}
				
				int result = sqlite3_step(book_insert);
				if(result != SQLITE_OK && result != SQLITE_ROW && result != SQLITE_DONE) throw -1;

				sqlite3_reset(book_insert);
				
				pages.add(pd);
				
				itemid++;
				
			}

			//cr->show_page();
			
		}	
		
		sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &errmsg);
		
		sqlite3_finalize(book_insert); 
		sqlite3_close(db);
		
		cout << "Done Loading" << endl; 
		
	}		
	
	void __load_sqlite(Pages &pages, path filename) {
		
		cout << "Loading SQLite" << endl; 
		
		sqlite3 * db; 
		int rc;

		rc = sqlite3_open(filename.c_str(), &db);

		if( rc ){
			//Failed to open
		}
		
		const string book_select_sql = "SELECT * FROM book";
		
		sqlite3_stmt * book_select; 
		
		rc = sqlite3_prepare_v2(db, book_select_sql.c_str(), -1, &book_select, 0);
		if(rc != SQLITE_OK && rc != SQLITE_DONE) throw -1;
		
		rc = sqlite3_step(book_select); 
		
		PageDescriptor pd;
		int currentpagen = 0; 
		
		while ( rc == SQLITE_ROW ) 
		{
			
			int pagen = sqlite3_column_int(book_select, 0);
			//int descriptorn = sqlite3_column_int(book_select, 1); 
			int contenttype = sqlite3_column_int(book_select, 2); 
			ustring content = ustring((char*)sqlite3_column_text(book_select, 3)); 
			 
			if(pagen != currentpagen) {
				//New page! wrap up the old page.
				
				pagen = currentpagen; 
				pages.add(pd);
				pd = PageDescriptor();
				
			}
			
			PageContentType pct;
			if(contenttype == 0) pct = PAGE_H1; 
			else if(contenttype == 1) pct = PAGE_H2; 
			else if(contenttype == 2) pct = PAGE_PARAGRAPH; 
			else pct = PAGE_FRAGMENT; 
			
			pd.items.push_back(PageContentItem(pct, content));
				
			//cout << content << endl;
			
			rc = sqlite3_step(book_select); 
			
		}

		if ( rc==SQLITE_ERROR)    
		{
			cout << "Database Error " << endl;
		}
		
		sqlite3_finalize(book_select); 
		
		sqlite3_close(db);
		
	}
	
} //end of anonymous namespace

void Paginator::load(Pages &pages, string filename) {
	
	Pango::init();
	
	//OK, time for heavy lifting;
	
	path file = path(filename);
	
	if(!exists(file)) {
		//do something
	}
	
	path fn = file.filename();
	
	path dbfn(".");
	dbfn += fn; 
	dbfn += path(".db");
	
	path databasefile = fn.parent_path();
	
	databasefile /= dbfn; 
	
	cout << databasefile << endl; 
	
	if(!exists(databasefile)) {
		//Urgh. we need to load the whole thing from the damn EPUB file.  
		thread t(__load_epub, ref(pages), file, databasefile);
		t.detach();
	}
	else {
		//Good, we can load from SQLite and we don't have to do any parsing. 
		thread t(__load_sqlite, ref(pages), databasefile);
		t.detach();
	}
			
}
