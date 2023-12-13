#pragma once

// std libs
#include <string>
#include <vector>

// Graphix_calc
#include "Graphix_calc/axis.h"
#include "Graphix_calc/input_box.h"

// Graph_lib
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Window.h>

using Graph_lib::Button;
using Graph_lib::Marks;
using Graph_lib::Point;
using Graph_lib::Vector_ref;
using Graphix_calc::Axis;
using Graphix_calc::Input_box;

using std::vector;

namespace Frontend {

// класс окна с осями и возможностью добавлять графики
class Graphix_window : public Graph_lib::Window
{
  public:
    Graphix_window(Point left_corner, pix_numb width, pix_numb height, const std::string& title,
                   pix_numb scale);

    // methods

    // ждём, когда нажмётся какая-нибудь кнопка
    void wait_for_button ()
    {
        while (!button_pushed && Fl::wait())
            ;
        button_pushed = false;
        Fl::redraw();
    }

    bool window_is_open () const { return !quit_button_pushed; }

  private:
    // variables

    // общее значение масштаба: количество пикселей в единичном отрезке
    pix_numb scale;
    // вертикальная и горизонтальная оси
    Axis *x_axis, *y_axis;
    // центр окна (используется только в Graph_lib::Function и методах на отображение точки)
    Point center;
    // вектор введенных пользователем строк
    vector<std::string> inputed_strings;
    // кнопка выхода из программы
    Button quit_button;
    // кнопка масштаб "+"
    Button incr_button;
    // кнопка масштаб "-"
    Button decr_button;
    Button new_graph_button;

    Button sh_points;
    Button hd_points;

    // вектор полей ввода
    vector<Input_box*> enter_menu;
    // общий массив со всеми сегментированными функциями (графиками)
    vector<Vector_ref<Graph_lib::Function>> graphics;

    Vector_ref<Marks> all_points;

    bool is_points_visible{false};
    bool button_pushed{false};
    bool quit_button_pushed{false};

    // methods

    // callbacks

    static void cb_quit (void*, void* widget);
    static void cb_incr (void*, void* widget);
    static void cb_decr (void*, void* widget);
    static void cb_draw (void*, void* widget);
    static void cb_hide (void*, void* widget);
    static void cb_rem (void*, void* widget);
    static void cb_new (void*, void* widget);
    static void cb_show_points (void*, void* widget);
    static void cb_hide_points (void*, void* widget);

    // функции, которые вызывают callbacks

    void quit ()
    {
        hide();
        quit_button_pushed = true;
    }

    void change_scale (pix_numb new_scale);

    void increase_scale ();

    void decrease_scale ();

    void draw_graph (size_t graph_number);

    void hide_graph (size_t graph_number);

    void rem_graph (size_t graph_number);

    void graph_clear (size_t graph_number);

    void new_graph ();

    void show_points ();

    void hide_points ();

    void clear_points ();
};

}  // namespace Frontend