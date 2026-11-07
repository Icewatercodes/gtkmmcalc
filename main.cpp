#include "calculator.h"
#include <gtkmm/application.h>
#include <iostream>
#include <deque>
#include <format>

const std::vector<std::string> labels = 
{
  "", "", "", "AC",
  "7", "8", "9", "",
   "4", "5", "6", "",
   "1", "2", "3", "",
   "0", " ", "=", "+"
};

Calculator::Calculator()
{
  set_name("window");
  //auto Controller = Gtk::ShortcutController::create();


  for (size_t i = 0; i < labels.size(); i++) {
    auto& btn = mainButtons.emplace_back(labels[i]);
    btn.set_size_request(50,50);
    btn.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(
            *this, 
            &Calculator::onButtonClicked), 
            labels[i]
          ));
    mainGrid.attach(btn, i%4, i/4+1);

  }

  mainGrid.attach(mainScreen, 0,0,4);
  mainScreen.set_use_markup(true);
  mainGrid.set_margin(30);
  mainGrid.set_row_spacing(10);
  mainGrid.set_column_spacing(10);
  set_child(mainGrid);

}

void Calculator::updateScreen()
{
  
  if(STATE == RESULT) {
    screenValue = doubleToDeque(result);
    STATE = FIRST;
  }

  Glib::ustring temp;
  for (size_t i = 0; i < screenValue.size(); i++) {
    temp.push_back(screenValue.at(i));
  }

  mainScreen.set_label("<span size=\"x-large\">" + temp + "</span>");
}

void Calculator::clearScreen() 
{
  while(!screenValue.empty()){
    screenValue.pop_back();
  }
  updateScreen();
}

std::deque<char> Calculator::doubleToDeque(double in)
{
  std::string str = std::to_string(in); 
  std::deque<char> out(str.begin(), str.end());
  return out;
}

double Calculator::dequeToDouble(const std::deque<char>& in)
{
  double out = 0;
  for (int i = 0; i < in.size(); i++) {
    std::cout << "i: " << int(i - '0') << '\n';
    out += int(in.at(i) - '0')*10^i;
  }
  return out;
}

void Calculator::calculateResult() 
{
  b = dequeToDouble(screenValue);
  std::cout << std::format("a: {}, b: {}\n", a, b);
  switch(OPERATOR){
    case PLUS:
      result = a+b;
      STATE = RESULT;
      updateScreen();
      std::cout << result << '\n';
    break;
  }
}

void Calculator::onButtonClicked(const std::string& label)
{
  //std::cout << label[0] << "\n";
  switch(label[0]){
    case 'A':
      clearScreen();
      updateScreen();
      STATE = FIRST;
      OPERATOR = NONE;
      //std::cout << std::format("a: %i b: %i", a, b);
      a,b,result = 0,0,0;
      break;

    case '+':
      if(STATE == FIRST) {
        a = dequeToDouble(screenValue);
        STATE = SECOND;
        OPERATOR = PLUS;
        clearScreen();
        updateScreen();
      } 
      break;

    case '=':
      calculateResult();
    break;

    

    default:
      if(screenValue.size() >= 10)
      {
        screenValue.pop_front();
      }
      screenValue.push_back(label[0]);
      updateScreen();
      break;
  }
}

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.example");

  //Shows the window and returns when it is closed.
  return app->make_window_and_run<Calculator>(argc, argv);
}
