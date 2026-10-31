#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <iostream>

class HelloWorldWindow : public Gtk::Window {
public:
    HelloWorldWindow() : hello_world("Hello World") {
        set_title("Hello World");
        set_child(hello_world);
        hello_world.signal_clicked().connect([]() {
            std::cout << "Hello world" << std::endl;
        });
        
    }
protected:
    Gtk::Button hello_world;
};

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.example");
    return app->make_window_and_run<HelloWorldWindow>(argc, argv);
}

