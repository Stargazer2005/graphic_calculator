#ifndef GRAPH_WIN_H
#define GRAPH_WIN_H

#include "back_func.h"
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Window.h>
#include <string>

using Graph_lib::Address;
using Graph_lib::Axis;
using Graph_lib::Button;
using Graph_lib::In_box;
using Graph_lib::Menu;
using Graph_lib::Point;
using Graph_lib::Window;

constexpr double scale_coef = 1.2;

class Graphic_window : public Window
{
public:
  Graphic_window(Point xy, int w, int h, const std::string& title,
                 int scale);

  void wait_for_button ()
  {
    while (!button_pushed && Fl::wait())
      ;
    button_pushed = false;
    Fl::redraw();
  }

  bool window_is_open () { return !quit_button_pushed; }

  std::string get_func () { return func_string; }

private:
  int s;
  Point center;
  Axis x_, y_;
  std::string func_string = "";

  Button quit_button;

  Menu zoom_menu;
  Button incr_button;
  Button decr_button;

  Button draw_button;
  In_box enter_box;

  bool button_pushed{false};
  bool quit_button_pushed{false};

  static void cb_quit (Address, Address widget);
  static void cb_incr (Address, Address widget);
  static void cb_decr (Address, Address widget);
  static void cb_draw (Address, Address widget);

  void quit ()
  {
    hide();
    quit_button_pushed = true;
  }

  void increase_scale ()
  {
    s = int(s * scale_coef);
    button_pushed = true;
  }

  void decrease_scale ()
  {
    s = int(s / scale_coef);
    button_pushed = true;
  }

  void draw_some_graph (std::string s);

  void draw_graph ()
  {
    func_string = enter_box.get_string();
    draw_some_graph(func_string);
    button_pushed = true;
  }
};
#endif  // GRAPH_WIN_H