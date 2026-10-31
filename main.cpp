#include "calculator.h"
#include <gtkmm/application.h>
#include <iostream>
#include <deque>

const std::vector<std::string> labels = {
  "7", "8", "9", "/",
   "4", "5", "6", "*",
   "1", "2", "3", "-",
   "0", ".", "=", "+"
};

Calculator::Calculator()
{
  set_name("window");
  
  for (size_t i = 0; i < labels.size(); i++) {
    auto& btn = mainButtons.emplace_back(labels[i]);
    btn.set_size_request(50,50);
    btn.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Calculator::onButtonClicked), labels[i]));
        mainGrid.attach(btn, i%4, i/4+1);
  }

  mainGrid.attach(mainScreen, 0,0,4);
  mainScreen.set_use_markup(true);
  mainGrid.set_margin(30);
  
  mainGrid.set_row_spacing(10);
  mainGrid.set_column_spacing(10);
  set_child(mainGrid);
}


void Calculator::updateScreen(const std::string& input)
{
  Glib::ustring temp;
  for (size_t i = 0; i < screenValue.size(); i++) {
    temp.push_back(screenValue.at(i));
  }

  mainScreen.set_label("<span size=\"xx-large\">" + temp + "</span>");
}

void Calculator::onButtonClicked(const std::string& label)
{
  if(screenValue.size() >= 10)
  {
    screenValue.pop_front();
  }
    screenValue.push_back(label[0]);
  updateScreen(label);
}

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.example");

  //Shows the window and returns when it is closed.
  return app->make_window_and_run<Calculator>(argc, argv);
}
