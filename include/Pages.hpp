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

#ifndef Paginator_H
#define Paginator_H

#include <cairomm/cairomm.h>
#include <pangomm.h>
#include <glibmm.h>
#include <string>
#include <vector>
//Stuff's getting serious:	
#include <mutex>             
#include <condition_variable>

#include <Epub.hpp>

using Glib::ustring;  

using std::mutex; 
using std::condition_variable; 

enum PageContentType {
	PAGE_H1, 
	PAGE_H2, 
	PAGE_PARAGRAPH, 
	PAGE_FRAGMENT
};

class PageContentItem {

	public:
		PageContentType pct; 
		ustring content; 
	
		PageContentItem(PageContentType _pct, ustring _content); 
		
		PageContentItem(PageContentItem const & cpy);
		PageContentItem(PageContentItem && mv) ;
		PageContentItem& operator =(const PageContentItem& cpy);
		PageContentItem& operator =(PageContentItem && mv) ;
			
		~PageContentItem(); 
	
};

class PageDescriptor {
	
	public: 
		vector<PageContentItem> items; 
	
		PageDescriptor(); 
		
		PageDescriptor(PageDescriptor const & cpy);
		PageDescriptor(PageDescriptor && mv) ;
		PageDescriptor& operator =(const PageDescriptor& cpy);
		PageDescriptor& operator =(PageDescriptor && mv) ;
			
		~PageDescriptor(); 	
	
};

class Pages {
	
	private:
		mutex mtx; 
		condition_variable ready; 
		vector<PageDescriptor> descriptors; 
		bool finished; 
	
		bool available(const unsigned int n) const; 	
	
	public: 
		Pages(); 
	
		Pages(Pages const & cpy) = delete;
		Pages(Pages && mv)  = delete;
		Pages& operator =(const Pages& cpy) = delete;
		Pages& operator =(Pages && mv) = delete;
			
		~Pages(); 		
		
		PageDescriptor get(const unsigned int n); 
		void add(PageDescriptor p);
	
};

class Paginator
{

	public:
		Paginator() = delete;
		virtual ~Paginator() = delete;
	
		static void load(Pages& pages, string filename); 
	
};

#endif // BOOKREADER_H