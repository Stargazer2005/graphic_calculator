#pragma once

#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Window.h>

#include "axis.h"

using Graph_lib::Button;
using Graph_lib::Point;

namespace Graphix_calc {

// класс окна с осями и возможностью добавлять графики
class Graphix_window : public Graph_lib::Window
{
  public:
    Graphix_window();

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
    // общее значение масштаба: количество пикселей в единичном отрезке
    pix_numb scale;
    Axis *x_axis, *y_axis;
    Point center;
    std::vector<std::string> func_strings;
    Button quit_button;

    Graph_lib::Menu zoom_menu;
    Button incr_button;
    Button decr_button;

    Button draw_button;
    Button new_button;

    Graph_lib::Vector_ref<Graph_lib::In_box> enter_menu;
    std::vector<Graph_lib::Vector_ref<Graph_lib::Function>> graphics;

    bool button_pushed{false};
    bool quit_button_pushed{false};

    static void cb_quit (void*, void* widget);
    static void cb_incr (void*, void* widget);
    static void cb_decr (void*, void* widget);
    static void cb_draw (void*, void* widget);
    static void cb_new (void*, void* widget);

    void quit ()
    {
        hide();
        quit_button_pushed = true;
    }

    void increase_scale ();

    void decrease_scale ();

    void draw_graphs ();

    void new_graph ();

    void draw_some_graph (std::string s);
};

}  // namespace Graphix_calc