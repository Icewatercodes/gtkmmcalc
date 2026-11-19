#include "calculator.h"
#include <gtkmm.h>
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

  for (size_t i = 0; i < labels.size(); i++) {
    const std::string& lbl = labels[i];
    auto& btn = mainButtons.emplace_back(lbl);
    btn.set_size_request(50,50);
    btn.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(
            *this, 
            &Calculator::onButtonClicked), 
            lbl
          ));
    mainGrid.attach(btn, i%4, i/4+1);

    if (lbl == "") continue;

    if(lbl == "="){
      keyToButtonIndex[GDK_KEY_equal] = i;
      keyToButtonIndex[GDK_KEY_Return] = i;
      keyToButtonIndex[GDK_KEY_KP_Enter] = i;
    } 
    else if (lbl == "AC") {
      keyToButtonIndex[GDK_KEY_Escape] = i;
      keyToButtonIndex[GDK_KEY_BackSpace] = i;
    } else {
      guint keyval = gdk_unicode_to_keyval((guint32)(unsigned char)lbl[0]);
      keyToButtonIndex[keyval] = i;
    }

  }

  mainGrid.attach(mainScreen, 0,0,4);
  mainScreen.set_use_markup(true);
  mainGrid.set_margin(30);
  mainGrid.set_row_spacing(10);
  mainGrid.set_column_spacing(10);
  set_child(mainGrid);

  auto controller = Gtk::EventControllerKey::create();
  controller->signal_key_pressed().connect(
      sigc::mem_fun(*this, &Calculator::onKeyPressed), false);
  controller->signal_key_released().connect(
      sigc::mem_fun(*this, &Calculator::onKeyReleased), false);
  add_controller(controller);
}

bool Calculator::onKeyPressed(guint keyval, guint /*keycode*/, Gdk::ModifierType /*state*/)
{
    auto it = keyToButtonIndex.find(keyval);
    if (it != keyToButtonIndex.end() && it->second < mainButtons.size()) {
        if (activeKey == 0) {
            activeKey = keyval;
            mainButtons[it->second].set_state_flags(Gtk::StateFlags::ACTIVE, false);
        }
        return true;
    }
    return false;
}

void Calculator::onKeyReleased(guint keyval, guint /*keycode*/, Gdk::ModifierType /*state*/)
{
    if (keyval == activeKey) {
        auto it = keyToButtonIndex.find(keyval);
        if (it != keyToButtonIndex.end() && it->second < mainButtons.size()) {
            auto& btn = mainButtons[it->second];
            btn.unset_state_flags(Gtk::StateFlags::ACTIVE);
            btn.activate(); // <-- Triggers signal_clicked() in gtkmm 4
        }
        activeKey = 0;
    }
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
  for (long unsigned int i = 0; i < in.size(); i++) {
    std::print("dequeToDouble: 1: {} 2: {}\n", in.at(in.size() - 1 -i), int(in.at(in.size()-1-i)) - '0');
    if(i == 0) {
      out += int(in.at(in.size() - 1 - i) - '0');
    } else {
      out += int(in.at(in.size() - 1 - i) - '0')*std::pow(10,i);
    }
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
<<<<<<< HEAD
    break;


    
=======
    return;
>>>>>>> 072b662a8a16723f540b5ecce479c4d373c75b26

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
