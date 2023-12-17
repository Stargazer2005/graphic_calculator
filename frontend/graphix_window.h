#pragma once

// std libs
#include <string>
using std::string;
#include <vector>
using std::vector;

// Graphix_calc
#include "Graphix_calc/Axis.h"
using Graphix_calc::Axis;
#include "Graphix_calc/Function_box.h"
using Graphix_calc::Function_box;
#include "Graphix_calc/Point_box.h"
using Graphix_calc::Point_box;
#include "Graphix_calc/Segmented_function.h"
using Graphix_calc::Segmented_function;

// Backend
#include "../backend.h"

// Graph_lib
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
using Graph_lib::Color;
using Graph_lib::Function;
#include <Graph_lib/Window.h>
using Graph_lib::Button;
using Graph_lib::Marks;
using Graph_lib::Vector_ref;

namespace Frontend {

// класс окна с осями и возможностью добавлять графики
class Graphix_window : public Graph_lib::Window
{
  public:
    Graphix_window(Graph_lib::Point left_corner, int width, int height, const string& title,
                   int scale);

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
    int scale;
    // начало координат в пикселях (используется только в Graph_lib::Function и методах на
    // отображение точки)
    Graph_lib::Point origin;
    // вертикальная и горизонтальная оси
    Axis *x_axis, *y_axis;
    // вектор введенных пользователем строк
    vector<string> inputed_funcs;
    // кнопка выхода из программы
    Button quit_button;
    // кнопка масштаб "+"
    Button incr_button;
    // кнопка масштаб "-"
    Button decr_button;
    // кнопка "создать новый график"
    Button new_button;

    // вектор полей ввода(группы поля ввода и трёх кнопок: draw, hide, remove)
    vector<Function_box*> enter_menu;

    // общий массив со всеми сегментированными функциями (графиками)
    vector<Vector_ref<Function>> graphics;
    // общий массив со всеми сегментированными производными функций (графиками)
    vector<Vector_ref<Function>> derivatives;

    // меню с общим полем для точек
    Point_box point_box;
    // общий массив всех точек на экране
    Vector_ref<Marks> all_points;

    bool is_points_visible{false};
    bool button_pushed{false};
    bool quit_button_pushed{false};

    // methods

    // "переименовали" методы Страуструпа, чтобы избежать путанницы

    int win_h () const { return y_max(); }

    int win_w () const { return x_max(); }

    // callbacks

    static void cb_quit (void*, void* widget);
    static void cb_incr_scale (void*, void* widget);
    static void cb_decr_scale (void*, void* widget);
    static void cb_draw_graph (void*, void* widget);
    static void cb_hide_graph (void*, void* widget);
    static void cb_draw_der (void*, void* widget);
    static void cb_hide_der (void*, void* widget);
    static void cb_rem_func (void*, void* widget);
    static void cb_new_func (void*, void* widget);
    static void cb_show_points (void*, void* widget);
    static void cb_hide_points (void*, void* widget);

    // функции, которые вызывают callbacks

    void quit ()
    {
        hide();

        quit_button_pushed = true;
    }

    void change_scale (int new_scale);

    void incr_scale ();

    void decr_scale ();

    // void update_graph (size_t func_index);

    void draw_graph (size_t func_index);

    // void update_der (size_t der_index);

    void draw_der (size_t der_index);

    void hide_graph (size_t func_index);

    void hide_der (size_t der_index);

    void rem_func (size_t func_index);

    // вспомогательная фунция, добавляющая в вектор введенных пользователем строк всё из enter_menu
    void fill_graphs ();

    // вспомогательная фунция, отчищающая память и удаляющая все строки из вектора введенных
    // пользователем строк
    void clear_graph (size_t func_index);

    void clear_der (size_t der_index);

    void cb_new_func ();

    // void update_points ();

    void show_points ();

    void hide_points ();

    // вспомогательная фунция, удаляющая все точки с экрана и чистящая память
    void clear_points ();
};

}  // namespace Frontend