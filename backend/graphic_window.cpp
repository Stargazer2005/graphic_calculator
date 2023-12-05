#include "graphic_window.h"
#include "analysis.h"
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <iostream>
#include <vector>

using namespace Graph_lib;

MyAxis::MyAxis(Orientation d, Point xy, int length, int scale, const std::string& lab)
    : dist{scale}, label{Point{0, 0}, lab}
{
  if (length < 0)
    error("bad axis length");
  switch (d)
  {
  case MyAxis::x:
  {
    Shape::add(Point{xy.x - length / 2, xy.y});
    Shape::add(xy);
    Shape::add(Point{xy.x + length / 2, xy.y});
    if (dist > 0)
    {
      int x1 = xy.x + dist;
      int x2 = xy.x - dist;
      for (int i = 0; i < length / (2 * dist); ++i)
      {
        notches.add(Point{x1, xy.y + 2}, Point{x1, xy.y - 2});
        x1 += dist;
        notches.add(Point{x2, xy.y + 2}, Point{x2, xy.y - 2});
        x2 -= dist;
      }
    }
    // label under the line
    label.move(length - 20, xy.y + 20);
    break;
  }
  case MyAxis::y:
  {
    Shape::add(Point{xy.x, xy.y + length / 2});
    Shape::add(xy);
    Shape::add(Point{xy.x, xy.y - length / 2});
    if (dist > 0)
    {
      int y1 = xy.y + dist;
      int y2 = xy.y - dist;
      for (int i = 0; i < length / (2 * dist); ++i)
      {
        notches.add(Point{xy.x - 2, y1}, Point{xy.x + 2, y1});
        y1 += dist;
        notches.add(Point{xy.x - 2, y2}, Point{xy.x + 2, y2});
        y2 -= dist;
      }
    }
    // label at top
    label.move(xy.x - 20, xy.y - length / 2 + 20);
    break;
  }
  case MyAxis::z:
    error("z axis not implemented");
  }
}

void MyAxis::draw_lines() const
{
  Shape::draw_lines();
  notches.draw();
  label.draw();
}

void MyAxis::set_color(Color c)
{
  Shape::set_color(c);
  notches.set_color(c);
  label.set_color(c);
}

Graphic_window::Graphic_window(Point xy, int w, int h, const std::string& title, int scale)
    : Window(xy, w, h, title), s{scale},
      x_axis{new MyAxis{MyAxis::Orientation::x, Point(x_max() / 2, y_max() / 2), x_max(), s, "X"}},
      y_axis{new MyAxis{MyAxis::Orientation::y, Point(x_max() / 2, y_max() / 2), y_max(), s, "Y"}},
      center{w / 2, h / 2}, quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit},
      zoom_menu{Point{w - 30, 20}, 30, 30, Graph_lib::Menu::Kind::vertical, ""},
      incr_button{Point{0, 0}, 30, 30, "+", cb_incr},
      decr_button{Point{0, 0}, 30, 30, "-", cb_decr},
      draw_button{Point{230, 0}, 70, 20, "Draw", cb_draw},
      new_button{Point{230, 20}, 70, 20, "New graph", cb_new}
{
  (*x_axis).set_color(Graph_lib::Color::Color_type::dark_cyan);
  attach(*x_axis);

  (*y_axis).set_color(Graph_lib::Color::Color_type::dark_cyan);
  attach(*y_axis);

  attach(quit_button);
  zoom_menu.attach(incr_button);
  zoom_menu.attach(decr_button);
  attach(zoom_menu);
  attach(draw_button);
  attach(new_button);
  for (int i = 0; i < 5; ++i)
  {
    func_strings.push_back("");
  }
  In_box* ib1 = new In_box{Point{30, 0}, 200, 30, "y = "};
  enter_menu.push_back(*ib1);
  attach(*ib1);
}

void Graphic_window::cb_quit(Address, Address widget)
{
  TRACE_FUNC;
  auto& btn = Graph_lib::reference_to<Button>(widget);
  dynamic_cast<Graphic_window&>(btn.window()).quit();
}

void Graphic_window::cb_incr(Address, Address widget)
{
  TRACE_FUNC;
  auto& btn = Graph_lib::reference_to<Button>(widget);
  dynamic_cast<Graphic_window&>(btn.window()).increase_scale();
}

void Graphic_window::cb_decr(Address, Address widget)
{
  TRACE_FUNC;
  auto& btn = Graph_lib::reference_to<Button>(widget);
  dynamic_cast<Graphic_window&>(btn.window()).decrease_scale();
}

void Graphic_window::cb_draw(Address, Address widget)
{
  TRACE_FUNC;
  auto& btn = Graph_lib::reference_to<Button>(widget);
  dynamic_cast<Graphic_window&>(btn.window()).draw_graph();
}

void Graphic_window::cb_new(Address, Address widget)
{
  TRACE_FUNC;
  auto& btn = Graph_lib::reference_to<Button>(widget);
  dynamic_cast<Graphic_window&>(btn.window()).new_graph();
}

void Graphic_window::draw_some_graph(std::string str)
{
  TRACE_FUNC;
  Vector_ref<Function> functions;
  std::vector<std::string> rev_pol = reverse_polish(lexeme(str));
  auto func = [&rev_pol] (double x) { return calc(rev_pol, x); };
  double prec_seg = (ceil(x_max() / (2 * s)) + 1) / (sqrt(s) * x_max() / 5);

  std::vector<Segment> seg = get_info(rev_pol, -ceil(x_max() / (2 * s)) - 1,
                                      ceil(x_max() / (2 * s)) + 1, (2 * y_max() / s) + 1, prec_seg);
  for (size_t i = 0; i < seg.size(); i++)
  {
    // std::cout << seg[i].start << " " << seg[i].stop << std::endl;
    double prec_func = (((seg[i].stop - seg[i].start + 2) / (2 * ceil(x_max() / (2 * s)) + 2))) *
                       (sqrt(s) * x_max() / 5);
    Graph_lib::Function* f1 = new Graph_lib::Function{func,      seg[i].start, seg[i].stop, center,
                                                      prec_func, double(s),    double(s)};
    functions.push_back(*f1);
    attach(*f1);
  }
  graphics.push_back(functions);
}

void Graphic_window::increase_scale()
{
  TRACE_FUNC;
  detach(*x_axis);
  detach(*y_axis);
  s = ceil(s * scale_coef);
  MyAxis* new_x_axis =
      new MyAxis{MyAxis::Orientation::x, Point(x_max() / 2, y_max() / 2), x_max(), s, "X"};
  (*new_x_axis).set_color(Graph_lib::Color::Color_type::dark_cyan);
  x_axis = new_x_axis;
  attach(*x_axis);
  MyAxis* new_y_axis =
      new MyAxis{MyAxis::Orientation::y, Point(x_max() / 2, y_max() / 2), y_max(), s, "Y"};
  (*new_y_axis).set_color(Graph_lib::Color::Color_type::dark_cyan);
  y_axis = new_y_axis;
  attach(*y_axis);
  draw_graph();
}

void Graphic_window::decrease_scale()
{
  TRACE_FUNC;
  if (s > 1)
  {
    detach(*x_axis);
    detach(*y_axis);
    s = floor(s / scale_coef);
    MyAxis* new_x_axis =
        new MyAxis{MyAxis::Orientation::x, Point(x_max() / 2, y_max() / 2), x_max(), s, "X"};
    (*new_x_axis).set_color(Graph_lib::Color::Color_type::dark_cyan);
    x_axis = new_x_axis;
    attach(*x_axis);
    MyAxis* new_y_axis =
        new MyAxis{MyAxis::Orientation::y, Point(x_max() / 2, y_max() / 2), y_max(), s, "Y"};
    (*new_y_axis).set_color(Graph_lib::Color::Color_type::dark_cyan);
    y_axis = new_y_axis;
    attach(*y_axis);
    draw_graph();
  }
}

void Graphic_window::draw_graph()
{
  TRACE_FUNC;
  for (int i = 0; i < graphics.size(); ++i)
    for (int j = 0; j < graphics[i].size(); j++)
      detach(graphics[i][j]);

  if (graphics.size() > 0)
    graphics.clear();  // очищает память тоже
  for (int i = 0; i < enter_menu.size(); ++i)
  {

    func_strings[i] = enter_menu[i].get_string();

    if (func_strings[i] != "")
    {
      draw_some_graph(func_strings[i]);
    }
  }
  button_pushed = true;
}

void Graphic_window::new_graph()
{
  TRACE_FUNC;
  In_box* new_ib = new In_box{Point{30, 30 * enter_menu.size()}, 200, 30, "y = "};
  enter_menu.push_back(*new_ib);
  attach(*new_ib);
  if (enter_menu.size() == 5)
  {
    detach(new_button);
  }
  button_pushed = true;
}
