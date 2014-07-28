#include "BookReader.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>

int main(int argc, char* argv[])
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  Gtk::Window window;
  window.set_title("Drawing text example");
	
  window.set_default_size(600,900);

  BookReader area;
  window.add(area);
  area.show();

  return app->run(window);
}