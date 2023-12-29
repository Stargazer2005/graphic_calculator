#include "Graphix_window.h"

// std libs
#include <stdexcept>
using std::stod;
using std::string;
using std::to_string;
using std::vector;

// Graph_lib
using Graph_lib::Button;
using Graph_lib::Color;
using Graph_lib::Marks;
using Graph_lib::Point;

// Graphix_calc
using namespace Graphix_calc;

// utility
#include "../utility/constants.h"
using namespace Frontend_consts;

namespace Graphix_win {

void Graphix_window::cb_incr_unit_intr(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).incr_unit_intr();
}

void Graphix_window::cb_decr_unit_intr(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).decr_unit_intr();
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

void Graphix_window::cb_change_unit_intr(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).change_unit_intr();
}

void Graphix_window::change_unit_intr()
{
    // домножаем пользовательский масштаб на длину единичного отрезка
    try
    {
        double new_unit_intr = stod(scale_box.get_string()) * mark_intr;
        update_unit_intr(new_unit_intr);
    }
    catch (...)
    {
        scale_box.put_string("invalid input");
    }

    button_pushed = true;
}

void Graphix_window::incr_unit_intr()
{
    update_unit_intr(unit_intr * unit_intr_coef);
    scale_box.put_string(to_string(unit_intr / mark_intr));

    button_pushed = true;
}

void Graphix_window::decr_unit_intr()
{
    update_unit_intr(unit_intr / unit_intr_coef);
    scale_box.put_string(to_string(unit_intr / mark_intr));

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
    clear_graphix(func_index, false);

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
    clear_deriv(der_index, false);

    enter_menu[der_index]->der_hide();

    if (enter_menu[der_index]->is_input_valid())
        enter_menu[der_index]->set_der_str("");

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

    graphics.push_back(new Segmented_Graphix());
    derivs.push_back(new Segmented_Graphix());

    inputed_strings.push_back(empty_str);
    inputed_funcs.push_back(empty_func);

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
        // двигаем их вверх и меняем номер на предыдущий
        enter_menu[j]->move(0, -func_box_h);
        enter_menu[j]->set_index(enter_menu[j]->get_index() - 1);
    }

    delete enter_menu[func_index];

    // также изменяем размеры самого вектора
    enter_menu.erase(enter_menu.begin() + func_index);

    inputed_strings.erase(inputed_strings.begin() + func_index);
    inputed_funcs.erase(inputed_funcs.begin() + func_index);

    // детачим этот график, его производную и чистим память
    clear_graphix(func_index);
    clear_deriv(func_index);

    graphics.erase(graphics.begin() + func_index);
    derivs.erase(derivs.begin() + func_index);

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

}  // namespace Graphix_win