#include "graphic_window.h"
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>

using Graph_lib::Address;
using Graph_lib::Axis;
using Graph_lib::In_box;
using Graph_lib::Menu;
using Graph_lib::Point;
using Graph_lib::Window;

Graphic_window::Graphic_window(Point xy, int w, int h,
                               const std::string& title, int scale)
    : Window(xy, w, h, title), s{scale}, center{w / 2, h / 2},
      x_(Axis::Orientation::x, Point(0, h / 2), w, w / s, ""),
      y_(Axis::Orientation::y, Point(w / 2, h), h, h / s, ""),
      quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit},
      zoom_menu{Point{0, 50}, 30, 30, Graph_lib::Menu::Kind::vertical, ""},
      incr_button{Point{0, 50}, 30, 30, "+", cb_incr},
      decr_button{Point{0, 80}, 30, 30, "-", cb_decr},
      draw_button{Point{0, 30}, 70, 20, "Draw", cb_draw},
      enter_box{Point{30, 0}, 200, 30, "y = "}
{
  attach(x_);
  attach(y_);
  attach(quit_button);
  zoom_menu.attach(incr_button);
  zoom_menu.attach(decr_button);
  attach(zoom_menu);
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

void Graphic_window::draw_some_graph(std::string s)
{
  auto func = [&s] (double x)
  { return calc(reverse_polish(lexeme(s)), x); };

  Graph_lib::Function f1{func, -20, 20, center, 4000, 25, 25};
  attach(f1);
}