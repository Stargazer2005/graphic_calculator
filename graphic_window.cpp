#include "graphic_window.h"
#include <Graph_Lib/Graph.h>
#include <Graph_lib/GUI.h>
#include <Graph_lib/Simple_window.h>

using namespace Graph_lib;

Graphic_window::Graphic_window(Point xy, int w, int h, const std::string& title, int scale)
    : Simple_window(xy, w, h, title), s{scale},
      x_(Axis::Orientation::x, Point(0, h / 2), w, w / s, ""),
      y_(Axis::Orientation::y, Point(w / 2, h), h, h / s, "")
{
    attach(x_);
    attach(y_);
}