#include "calculator.h"
#include <gtkmm/application.h>
#include <iostream>

HelloWorld::HelloWorld()
{
  // Sets the margin around the button.
  m_button.set_margin(50);
  //m_button.set_size_request(300,300);
  m_button.set_label("button");
  set_name("window");
  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  m_button.signal_clicked().connect([]() {
      std::cout << "HelloWorld" << '\n';  
    });

  //m_Frame.set_size_request(600,900);

  // This packs the button into the Window (a container).
  m_Frame.set_child(m_button);
  set_child(m_Frame);
}



int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.example");

  //Shows the window and returns when it is closed.
  return app->make_window_and_run<HelloWorld>(argc, argv);
}
