#ifndef GRAPH_WIN_H
#define GRAPH_WIN_H

#include "back_func.h"
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Window.h>
#include <string>
#include <tracer/tracer.h>
#include <vector>

using namespace Graph_lib;

constexpr double scale_coef = 1.2;

struct MyAxis : Shape
{
  enum Orientation
  {
    x,
    y,
    z
  };

  MyAxis(Orientation d, Point xy, int length, int scale = 0, const std::string& label = "");

  void draw_lines () const override;

  void set_color (Color c);
  int n;
  int dist;
  Text label;
  Lines notches;
};

class Graphic_window : public Graph_lib::Window

{
public:
  Graphic_window(Point xy, int w, int h, const std::string& title, int scale);

  void wait_for_button ()
  {
    while (!button_pushed && Fl::wait())
      ;
    button_pushed = false;
    Fl::redraw();
  }

  bool window_is_open () { return !quit_button_pushed; }

  std::vector<std::string> get_func () { return func_strings; }

private:
  int s;
  MyAxis *x_axis, *y_axis;
  Point center;
  std::vector<std::string> func_strings;
  Button quit_button;

  Menu zoom_menu;
  Button incr_button;
  Button decr_button;

  Button draw_button;
  Button new_button;

  Graph_lib::Vector_ref<In_box> enter_menu;
  // Graph_lib::Vector_ref<Function> graphics;
  std::vector<Graph_lib::Vector_ref<Function>> graphics;

  bool button_pushed{false};
  bool quit_button_pushed{false};

  static void cb_quit (Address, Address widget);
  static void cb_incr (Address, Address widget);
  static void cb_decr (Address, Address widget);
  static void cb_draw (Address, Address widget);
  static void cb_new (Address, Address widget);

  void quit ()
  {
    TRACE_FUNC;
    hide();
    quit_button_pushed = true;
  }

  void increase_scale ();

  void decrease_scale ();

  void draw_graph ();

  void new_graph ();

  void draw_some_graph (std::string s);
};
#endif  // GRAPH_WIN_H