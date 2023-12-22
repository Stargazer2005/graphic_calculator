#pragma once

// std libs
#include <string>
#include <vector>

// Graphix_calc
#include "../../backend/function.h"
#include "../Graphix_calc/Axis.h"
#include "../Graphix_calc/Dext_box.h"
#include "../Graphix_calc/Function_box.h"
#include "../Graphix_calc/Graphix.h"
#include "../Graphix_calc/Point_box.h"
#include "../Graphix_calc/Segmented_Graphix.h"

// Backend
#include "../../backend.h"

// Graph_lib
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Window.h>

// temp
#include "../temp_help.h"

namespace Frontend {

// класс окна с осями и возможностью добавлять графики
class Graphix_window : public Graph_lib::Window
{
  public:
    Graphix_window(Graph_lib::Point left_corner, int width, int height, const std::string& title,
                   double scale);

    // methods

    // ждём, когда нажмётся какая-нибудь кнопка
    void wait_for_button ()
    {
        while (!button_pushed && Fl::wait())
            ;

        cout << "some button pushed" << endl;
        cout << "inputed_strings: ";
        print(inputed_strings);
        cout << "inputed_func_strs: ";
        print(inputed_funcs);
        cout << endl << endl;

        button_pushed = false;
        Fl::redraw();
    }

    bool window_is_open () const { return !quit_button_pushed; }

  private:
    // variables

    // общее значение масштаба: количество пикселей в единичном отрезке!?
    double scale;
    // начало координат в пикселях (используется только в Graph_lib::Graphix и методах на
    // отображение точки)
    Graph_lib::Point origin;
    // линия - ограничитель системы координат от меню ввода
    Graph_lib::Line border;
    // вертикальная и горизонтальная оси
    Graphix_calc::Axis *x_axis, *y_axis;
    // кнопка выхода из программы
    Graph_lib::Button quit_button;
    // кнопка масштаб "+"
    Graph_lib::Button incr_button;
    // кнопка масштаб "-"
    Graph_lib::Button decr_button;
    // кнопка "создать новый график"
    Graph_lib::Button new_button;

    // вектор полей ввода(группы поля ввода и трёх кнопок: draw, hide, remove)
    std::vector<Graphix_calc::Function_box*> enter_menu;

    // общий вектор введенных пользователем строк
    std::vector<std::string> inputed_strings;
    std::vector<Backend::function> inputed_funcs;
    // общий вектор со всеми сегментированными функциями (графиками)
    std::vector<std::vector<Graphix_calc::Graphix*>> graphics;
    // общий вектор со всеми сегментированными производными функций (графиками)
    std::vector<std::vector<Graphix_calc::Graphix*>> derivs;
    // ветор, содержащий все зависимости функций друг от друга
    // vector<vector<size_t>> dependencies;

    // меню с общим полем для точек
    Graphix_calc::Point_box point_box;
    // общий массив всех точек на экране
    std::vector<Graph_lib::Marks*> all_points;

    Graph_lib::Button scale_button;

    Graphix_calc::Dext_box db;

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
    static void cb_change_scale (void*, void* widget);

    // функции, которые вызывают callbacks

    void quit ()
    {
        hide();

        quit_button_pushed = true;
    }

    // вспомогательная функция, меняющая текущий масштаб
    void update_scale (double new_scale);

    void change_scale ();

    void incr_scale ();

    void decr_scale ();

    // вспомогательная функция, полностью перерисовывающая график (без нажатия кнопки)
    void update_graphix (size_t func_index);

    // вспомогательная фунция, отчищающая память и удаляющая строку из вектора введенных
    // пользователем строк
    void clear_graphix (size_t func_index);

    void draw_graphix (size_t func_index);

    void hide_graphix (size_t func_index);

    // вспомогательная функция, полностью перерисовывающая производную (без нажатия кнопки))
    void update_deriv (size_t der_index);

    void clear_deriv (size_t der_index);

    void draw_deriv (size_t der_index);

    void hide_deriv (size_t der_index);

    void rem_func_box (size_t func_index);

    // вспомогательная фунция, добавляющая в вектор введенных пользователем строк тек
    void update_inputed_func (size_t func_index, bool need_update_strings = true);

    void clear_inputed_func (size_t func_index);

    // вспомогательная фунция, добавляющая в вектор введенных пользователем строк всё из enter_menu
    void fill_inputed_funcs ();

    void new_func_box ();

    void update_points ();

    // вспомогательная фунция, удаляющая все точки с экрана и чистящая память
    void clear_points ();

    void show_points ();

    void hide_points ();
};

}  // namespace Frontend