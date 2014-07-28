#include "BookReader.hpp"
#include "MainWindow.hpp"

#include <gtkmm/application.h>
#include <gtkmm/window.h>

using namespace Glib;
using namespace Gtk;

HeaderBar * header_bar;

int main(int argc, char* argv[])
{
	/*Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	Gtk::Window window;
	window.set_title("Drawing text example");

	window.set_default_size(600,900);

	BookReader area;
	window.add(area);
	area.show();

	return app->run(window); */
	
	RefPtr<Application> app = Application::create(argc, argv, "org.rmarti.dnd");

	MainWindow window;

	// Shows the window and returns when it is closed.
	return app->run(window);
}