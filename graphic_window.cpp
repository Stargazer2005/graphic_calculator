#include "graphic_window.h"
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Window.h>

using Graph_lib::Address;
using Graph_lib::Axis;
using Graph_lib::In_box;
using Graph_lib::Point;
using Graph_lib::Window;

Graphic_window::Graphic_window(Point xy, int w, int h, const std::string& title, int scale)
    : Window(xy, w, h, title), s{scale}, x_(Axis::Orientation::x, Point(0, h / 2), w, w / s, ""),
      y_(Axis::Orientation::y, Point(w / 2, h), h, h / s, ""),
      quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit},
      incr_button{Point{0, 70}, 50, 50, "+", cb_incr},
      decr_button{Point{0, 120}, 50, 50, "-", cb_decr},
      draw_button{Point{0, 50}, 70, 20, "Draw", cb_draw},
      enter_box{Point{80, 0}, 100, 50, "Enter func:"}
{
  attach(x_);
  attach(y_);
  attach(quit_button);
  attach(incr_button);
  attach(decr_button);
  attach(draw_button);
  attach(enter_box);
}

void Graphic_window::cb_quit(Address, Address widget)
{
  auto& btn = Graph_lib::reference_to<Button>(widget);
  dynamic_cast<Graphic_window&>(btn.window()).quit();
}

void Graphic_window::cb_incr(Address, Address widget)
{
  auto& btn = Graph_lib::reference_to<Button>(widget);
  dynamic_cast<Graphic_window&>(btn.window()).increase_scale();
}

void Graphic_window::cb_decr(Address, Address widget)
{
  auto& btn = Graph_lib::reference_to<Button>(widget);
  dynamic_cast<Graphic_window&>(btn.window()).decrease_scale();
}

void Graphic_window::cb_draw(Address, Address widget)
{
  auto& btn = Graph_lib::reference_to<Button>(widget);
  dynamic_cast<Graphic_window&>(btn.window()).draw_graph();
}
