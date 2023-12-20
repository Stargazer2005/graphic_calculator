#pragma once

// Graph_lib
#include <Graph_lib/GUI.h>
#include <Graph_lib/Point.h>
#include <Graph_lib/Window.h>

namespace Graphix_calc {

// класс поля, являющееся и вводом и выводом
class Dext_box : public Graph_lib::Widget
{
  public:
    Dext_box(Graph_lib::Point xy, int w, int h, const std::string& s);

    double get_int ();
    void put_double (double);

    void attach (Graph_lib::Window& win);
};

}  // namespace Graphix_calc