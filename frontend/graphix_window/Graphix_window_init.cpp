#include "Graphix_window.h"

// std libs
#include <stdexcept>
using std::string;
using std::vector;

// Graph_lib
using Graph_lib::Color;
using Graph_lib::Point;

// Graphix_calc
using namespace Graphix_calc;

// utility
#include "../utility/constants.h"
using namespace Frontend_consts;

namespace Graphix_win {

Graphix_window::Graphix_window()
    : Graph_lib::Window({100, 100}, 1000, 700, "FIDocalcus"), unit_intr{double(50)},
      // точка начала системы координат смещена вправо, чтобы графики и оси не заезжали на меню
      origin{(w() + func_box_w) / 2, h() / 2}, border{Point{func_box_w, 0}, Point{func_box_w, h()}},
      // кнопка quit находится в левом верхнем углу
      quit_button{Graph_lib::Point{w() - btn_w, 0}, btn_w, btn_h, "Quit", cb_quit},
      // кнопки изменения масштаба находятся справа и являются квадратами
      incr_button{Graph_lib::Point{w() - scl_btn_side, 0 + btn_h}, scl_btn_side, scl_btn_side, "+",
                  cb_incr_unit_intr},
      decr_button{Graph_lib::Point{w() - scl_btn_side, 0 + btn_h + scl_btn_side}, scl_btn_side,
                  scl_btn_side, "-", cb_decr_unit_intr},
      // а кнопка New находится правее меню
      new_button{Graph_lib::Point{in_box_w + scl_btn_side, 0}, btn_w, btn_h, "New", cb_new_func},
      // меню с точками
      point_box{w(), h(), cb_show_points, cb_hide_points},
      unit_intr_button{Graph_lib::Point{in_box_w + in_box_lab_w - btn_w, h() - in_box_h}, btn_w,
                       in_box_h, "Scale", cb_change_unit_intr},
      scale_box{Graph_lib::Point{in_box_lab_w, h() - in_box_h}, in_box_w - btn_w, in_box_h, "1:"}
{
    init();
}

Graphix_window::Graphix_window(Graph_lib::Point left_corner, pix_amount _width, pix_amount _height,
                               const string& title, pix_amount _unit_intr)
    : Graph_lib::Window(left_corner, _width, _height, title), unit_intr{double(_unit_intr)},
      // точка начала системы координат смещена вправо, чтобы графики и оси не заезжали на меню
      origin{(w() + func_box_w) / 2, h() / 2}, border{Point{func_box_w, 0}, Point{func_box_w, h()}},
      // кнопка quit находится в левом верхнем углу
      quit_button{Graph_lib::Point{w() - btn_w, 0}, btn_w, btn_h, "Quit", cb_quit},
      // кнопки изменения масштаба находятся справа и являются квадратами
      incr_button{Graph_lib::Point{w() - scl_btn_side, 0 + btn_h}, scl_btn_side, scl_btn_side, "+",
                  cb_incr_unit_intr},
      decr_button{Graph_lib::Point{w() - scl_btn_side, 0 + btn_h + scl_btn_side}, scl_btn_side,
                  scl_btn_side, "-", cb_decr_unit_intr},
      // а кнопка New находится правее меню
      new_button{Graph_lib::Point{in_box_w + scl_btn_side, 0}, btn_w, btn_h, "New", cb_new_func},
      // меню с точками
      point_box{w(), h(), cb_show_points, cb_hide_points},
      unit_intr_button{Graph_lib::Point{in_box_w + in_box_lab_w - btn_w, h() - in_box_h}, btn_w,
                       in_box_h, "Scale", cb_change_unit_intr},
      scale_box{Graph_lib::Point{in_box_lab_w, h() - in_box_h}, in_box_w - btn_w, in_box_h, "1:"}
{
    init();
}

void Graphix_window::init()
{
    // TODO: огромный список из конструктора тоже бы сюда навернуть
    // IDK: но что делать с тем, что фигуры нельзя копировать?..

    // не даём пользователю менять размеры окна
    size_range(w(), h(), w(), h());

    // задаём цвет окну
    // this->color(Color::white);

    inputed_strings.push_back(empty_str);
    inputed_funcs.push_back(empty_func);

    x_axis = new Axis{
        Axis::Orientation::horisontal, origin, w() - func_box_w, unit_intr, mark_intr, "X"};
    y_axis = new Axis{Axis::Orientation::vertical, origin, h(), unit_intr, mark_intr, "Y"};

    // задаём цвет осям
    x_axis->set_color(Color::Color_type::dark_cyan);
    y_axis->set_color(Color::Color_type::dark_cyan);

    Function_box* func_box =
        new Function_box{0, cb_draw_graph, cb_hide_graph, cb_rem_func, cb_draw_der, cb_hide_der};

    // и добавляем его в общий список всех Function_box'ов
    enter_menu.push_back(func_box);

    graphics.push_back(new Segmented_Graphix());
    derivs.push_back(new Segmented_Graphix());

    initial_attach();
}

void Graphix_window::initial_attach()
{
    attach(border);
    attach(*x_axis);
    attach(*y_axis);
    attach(quit_button);
    attach(incr_button);
    attach(decr_button);
    attach(new_button);
    attach(unit_intr_button);
    attach(scale_box);

    attach(*enter_menu[0]);
    attach(point_box);
}

}  // namespace Graphix_win