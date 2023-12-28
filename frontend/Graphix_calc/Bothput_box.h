#pragma once

// std libs
#include <string>

// Graph_lib
#include "../Graph_lib/GUI.h"
#include "../Graph_lib/Point.h"
#include "../Graph_lib/Window.h"

namespace Graphix_calc {

// класс поля, являющееся и вводом и выводом
class Bothput_box : public Graph_lib::Widget
{
  public:
    Bothput_box(Graph_lib::Point xy, pix_amount _width, pix_amount _height, const std::string& s);

    std::string get_string ();
    void put_string (const std::string&);

    void attach (Graph_lib::Window& win);
};

}  // namespace Graphix_calc