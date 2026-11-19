clear
g++ main.cpp -o my_app -std=c++23 (pkg-config --cflags --libs gtkmm-4.0 | string split ' ') && ./my_app
