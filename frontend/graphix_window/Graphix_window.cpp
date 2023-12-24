
#include "Graphix_window.h"

// std libs
#include <stdexcept>
using std::string;
using std::vector;

// Graph_lib
using Graph_lib::Button;
using Graph_lib::Color;
using Graph_lib::Marks;
using Graph_lib::Point;

// Graphix_calc
using namespace Graphix_calc;

// servant
#include "../servant/constants.h"
using namespace Front_consts;

namespace Frontend {

// из-за количества кнопок на экране конструктор сильно перегружен, но что поделать :)
Graphix_window::Graphix_window(Graph_lib::Point left_corner, int width, int height,
                               const string& title, double scale)
    : Graph_lib::Window(left_corner, width, height, title), scale{scale},
      // точка начала системы координат смещена вправо, чтобы графики и оси не заезжали на меню
      origin{(width + func_box_w) / 2, height / 2},
      border{Point{func_box_w, 0}, Point{func_box_w, height}},
      x_axis{new Axis{Axis::Orientation::horisontal, origin, width - func_box_w, scale, "X"}},
      y_axis{new Axis{Axis::Orientation::vertical, origin, height, scale, "Y"}},
      // кнопка quit находится в левом верхнем углу
      quit_button{Graph_lib::Point{width - btn_w, 0}, btn_w, btn_h, "Quit", cb_quit},
      // кнопки изменения масштаба находятся справа и являются квадратами
      incr_button{Graph_lib::Point{width - scl_btn_side, 0 + btn_h}, scl_btn_side, scl_btn_side,
                  "+", cb_incr_scale},
      decr_button{Graph_lib::Point{width - scl_btn_side, 0 + btn_h + scl_btn_side}, scl_btn_side,
                  scl_btn_side, "-", cb_decr_scale},
      // а кнопка New находится правее меню
      new_button{Graph_lib::Point{in_box_w + scl_btn_side, 0}, btn_w, btn_h, "New", cb_new_func},
      // меню с точками
      point_box{width, height, cb_show_points, cb_hide_points},
      scale_button{Graph_lib::Point{in_box_w + in_box_lab_w - btn_w, height - in_box_h}, btn_w,
                   in_box_h, "Scale", cb_change_scale},
      db{Graph_lib::Point{in_box_lab_w, height - in_box_h}, in_box_w - btn_w, in_box_h, "1:"}
{
    // не даём пользователю менять размеры окна
    size_range(width, height, width, height);

    // задаём цвет окну
    // this->color(Color::white);

    // инициализируем вектор введенных строк (пустыми)
    for (size_t i = 0; i < max_functions_amount; i++)
    {
        inputed_strings.push_back(empty_str);
        inputed_funcs.push_back(empty_func);
    }

    // задаём цвет осям
    x_axis->set_color(Color::Color_type::dark_cyan);
    y_axis->set_color(Color::Color_type::dark_cyan);

    // поскольку enter_menu не была инициализирована, а значит, мы создаем нулевой по
    // счету Function_box
    Function_box* func_box =
        new Function_box{0, cb_draw_graph, cb_hide_graph, cb_rem_func, cb_draw_der, cb_hide_der};
    func_box->set_index(0);

    // и добавляем его в общий список всех Function_box'ов
    enter_menu.push_back(func_box);

    // аттачим всё
    attach(border);
    attach(*x_axis);
    attach(*y_axis);
    attach(quit_button);
    attach(incr_button);
    attach(decr_button);
    attach(new_button);
    attach(scale_button);
    attach(db);

    attach(*func_box);
    attach(point_box);
}

void Graphix_window::cb_incr_scale(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).incr_scale();
}

void Graphix_window::cb_decr_scale(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).decr_scale();
}

void Graphix_window::cb_draw_graph(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).draw_graphix(btn.get_number());
}

void Graphix_window::cb_hide_graph(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).hide_graphix(btn.get_number());
}

void Graphix_window::cb_draw_der(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).draw_deriv(btn.get_number());
}

void Graphix_window::cb_hide_der(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).hide_deriv(btn.get_number());
}

void Graphix_window::cb_quit(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).quit();
}

void Graphix_window::cb_rem_func(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).rem_func_box(btn.get_number());
}

void Graphix_window::cb_new_func(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).new_func_box();
}

void Graphix_window::cb_show_points(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).show_points();
}

void Graphix_window::cb_hide_points(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).hide_points();
}

void Graphix_window::cb_change_scale(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).change_scale();
}

void Graphix_window::change_scale()
{
    // домножаем пользовательский масштаб на длину единичного отрезка
    int new_scale = db.get_int() * distance;
    update_scale(new_scale);

    button_pushed = true;
}

void Graphix_window::incr_scale()
{
    update_scale(scale * scale_coef);
    db.put_double(scale / distance);

    button_pushed = true;
}

void Graphix_window::decr_scale()
{
    update_scale(scale / scale_coef);
    db.put_double(scale / distance);

    button_pushed = true;
}

void Graphix_window::draw_graphix(size_t func_index)
{
    update_graphix(func_index);

    enter_menu[func_index]->graph_show();

    button_pushed = true;
}

void Graphix_window::hide_graphix(size_t func_index)
{
    clear_graphix(func_index);

    // прячем график
    enter_menu[func_index]->graph_hide();

    button_pushed = true;
}

void Graphix_window::draw_deriv(size_t der_index)
{
    update_deriv(der_index);

    enter_menu[der_index]->der_show();

    if (enter_menu[der_index]->is_input_valid())
        enter_menu[der_index]->set_der_str("(" + inputed_funcs[der_index].get_func_str() + ")'");

    button_pushed = true;
}

void Graphix_window::hide_deriv(size_t der_index)
{
    clear_deriv(der_index);

    enter_menu[der_index]->der_hide();

    button_pushed = true;
}

void Graphix_window::show_points()
{
    update_points();
    is_points_visible = true;

    button_pushed = true;
}

void Graphix_window::hide_points()
{
    clear_points();
    is_points_visible = false;

    button_pushed = true;
}

void Graphix_window::new_func_box()
{
    // если до увеличения массив function_боксов был пуст, то мы обратно двигаем new_button
    if (enter_menu.empty())
        new_button.move(func_box_w, 0);

    // создаем новый бокс (с индексом последний + 1 (= размер))
    Function_box* func_box = new Function_box{enter_menu.size(), cb_draw_graph, cb_hide_graph,
                                              cb_rem_func,       cb_draw_der,   cb_hide_der};

    // задаём ему последний номер и аттачим его
    func_box->set_index(enter_menu.size());
    attach(*func_box);

    // добавляем новый function_бокс в соотв. вектор
    enter_menu.push_back(func_box);

    // увеличиваем размер вектора с графиками (резервируем под новую сегментированную функцию)
    // FIXME: нужно написать конструктор по умолчанию для всех наших классов...
    graphics.push_back(vector<Graphix*>{});

    // если количество графиков стало максимально, скрываем кнопку "new_button"
    if (enter_menu.size() == max_functions_amount)
        detach(new_button);

    button_pushed = true;
}

void Graphix_window::rem_func_box(size_t func_index)
{
    // детачим кнопку под переданным номером
    enter_menu[func_index]->detach(*this);

    // проходимся по всем function_боксам начиная со следующего
    for (size_t j = func_index + 1; j < enter_menu.size(); ++j)
    {
        // двигаем их вверх и меняем номер на пердыдущий
        enter_menu[j]->move(0, -func_box_h);
        enter_menu[j]->set_index(enter_menu[j]->get_index() - 1);
    }

    // также изменяем размеры самого вектора
    enter_menu.erase(enter_menu.begin() + func_index);

    // детачим этот график, его производную и чистим память
    clear_graphix(func_index);
    clear_deriv(func_index);

    // также изменяем размеры самого вектора
    if (func_index > 0)
        graphics.erase(graphics.begin() + func_index);
    else
        graphics.clear();

    // FIXME: почему-то не работает erase graphics.erase(graphics.begin())

    // возвращаем кнопку "new_button" на экран, если был удален хоть один
    if (enter_menu.size() == max_functions_amount - 1)
        attach(new_button);

    // если не осталось полей для ввода, двигаем "new_button"
    if (enter_menu.empty())
        new_button.move(-func_box_w, 0);

    // в случае удаления одного из графиков удаляем все точки
    clear_points();

    button_pushed = true;
}

}  // namespace Frontend