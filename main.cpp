#include "calculator.h"
#include <gtkmm/application.h>
#include <iostream>
#include <deque>
#include <format>
#include <print>
#include <chrono>
#include <thread>

const std::vector<std::string> labels = 
{
  "", "", "", "AC",
  "7", "8", "9", "/",
   "4", "5", "6", "*",
   "1", "2", "3", "-",
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
  try {
    for (long unsigned int i = 0; i < in.size(); i++) {
      std::print("dequeToDouble: 1: {} 2: {}\n", in.at(in.size() - 1 -i), int(in.at(in.size()-1-i)) - '0');
      if(i == 0) {
        out += int(in.at(in.size() - 1 - i) - '0');
      } else {
        out += int(in.at(in.size() - 1 - i) - '0')*std::pow(10,i);
      }
    }
  } catch (std::out_of_range) {
    std::print("error std::out_of_range");
    out = 0;
    resetCalc();
    screenValue = {'e','r','r','o','r'};
    updateScreen();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    screenValue = {};
    updateScreen();
  }
  std::print("out: {}\n\n", out);
  return out;
}

void Calculator::resetCalc() {
        clearScreen();
      updateScreen();
      STATE = FIRST;
      OPERATOR = NONE;
      a = 0; b = 0; result = 0;
}

void Calculator::calculateResult() 
{
  b = dequeToDouble(screenValue);
  std::cout << std::format("a: {}, b: {}\n", a, b);
  switch(OPERATOR){
    case PLUS:
      result = a+b;
    break;
    case MINUS:
      result = a-b;
    break;
    case TIMES:
      result = a*b;
    break;
    case DIVIDE:
      result = a / b;
    break;
    default:
    break;
  }
  STATE = RESULT;
  updateScreen();
  std::cout << "result: "<< result << '\n';
}

void Calculator::onButtonClicked(const std::string& label)
{
  switch(label[0]){
    case 'A':
      resetCalc();
    return;

    case '+':
      OPERATOR = PLUS;
    break;

    case '-':
      OPERATOR = MINUS;
    break;

    case '*':
      OPERATOR = TIMES;
    break;

    case '/':
      OPERATOR = DIVIDE;
    break;

    case '=':
      calculateResult();
    return;

    default:
      if(screenValue.size() >= 10)
      {
        screenValue.pop_front();
      }
      screenValue.push_back(label[0]);
      updateScreen();
    return;
  }

  if(STATE == FIRST){
    a = dequeToDouble(screenValue);
    STATE = SECOND;
    clearScreen();
    updateScreen();
  }
}

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.example");

  //Shows the window and returns when it is closed.
  return app->make_window_and_run<Calculator>(argc, argv);
}
