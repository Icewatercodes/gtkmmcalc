#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/frame.h>
//#include <gtkmm/grid>
class HelloWorld : public Gtk::Window
{

public:
  HelloWorld();

protected:

  void on_button_clicked();
  //Member widgets
  Gtk::Button m_button;
  Gtk::Frame m_Frame;
  Gtk::Frame buttons_Frame;
  Gtk::Frame label_Frame;
};

#endif
