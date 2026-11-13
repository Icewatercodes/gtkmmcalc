#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/frame.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include "gtkmm/shortcutcontroller.h"

class Calculator : public Gtk::Window
{

public:
  Calculator();

protected:

  
  void updateScreen();
  void onButtonClicked(const std::string& label);
  void clearScreen();
  void calculateResult();
  void resetCalc();

  enum CALCPosibleStates {
    FIRST,
    SECOND,
    RESULT,
    
  };

  enum CALCOperators {
    NONE,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE
  };
  enum CALCPosibleStates STATE = FIRST;
  enum CALCOperators OPERATOR = NONE;

  double a, b, result;
  std::deque<char> screenValue;
  std::deque<char> doubleToDeque(double in = 0);
  double dequeToDouble(const std::deque<char>& in = {});

  Gtk::Label mainScreen;
  std::vector<Gtk::Button> mainButtons;
  Gtk::Grid mainGrid;
  //auto Controller;
  
};

#endif
