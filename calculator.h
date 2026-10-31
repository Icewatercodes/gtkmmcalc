#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/frame.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
class Calculator : public Gtk::Window
{

public:
  Calculator();

protected:

  double a, b, result;
  std::deque<char> screenValue;
  void updateScreen(const std::string& label);
  void onButtonClicked(const std::string& label);

  Gtk::Label mainScreen;
  std::vector<Gtk::Button> mainButtons;
  Gtk::Grid mainGrid;
};

#endif
