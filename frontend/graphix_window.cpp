#include <Graph_lib/Window.h>

#include "graphix_window.h"
#include "segmented_function.h"

#include "servant/constants.h"

using pix_numb = int;

using namespace Graph_lib;
using namespace Front_consts;
using Graphix_calc::Numbered_button;

namespace Graphix_calc {

Graphix_window::Graphix_window(Point xy, pix_numb w, pix_numb h, const std::string& title,
                               pix_numb scale)
    : Window(xy, w, h, title), scale{scale},
      x_axis{new Graphix_calc::Axis{Graphix_calc::Axis::Orientation::x,
                                    Point(x_max() / 2, y_max() / 2), x_max(), scale, "X"}},
      y_axis{new Graphix_calc::Axis{Graphix_calc::Axis::Orientation::y,
                                    Point(x_max() / 2, y_max() / 2), y_max(), scale, "Y"}},
      center{w / 2, h / 2}, quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit},
      incr_button{Point{w - 30, 20}, 30, 30, "+", cb_incr},
      decr_button{Point{w - 30, 50}, 30, 30, "-", cb_decr},
      new_button{Point{230, 0}, 70, 20, "New graph", cb_new}
{
    // size_range(w, h, w, h);  // Фиксируем масштаб

    x_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
    attach(*x_axis);

    y_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
    attach(*y_axis);

    attach(quit_button);
    attach(incr_button);
    attach(decr_button);
    attach(new_button);

    for (short i = 0; i < graphs_number; ++i)
        func_strings.push_back("");

    Input_box new_input;
    In_box* new_ib = new In_box{Point{30, 0}, 200, 30, "y = "};
    attach(*new_ib);
    Numbered_button* new_draw_button = new Numbered_button{Point{0, 30}, 70, 20, "Draw", cb_draw};
    new_draw_button->set_number(0);
    attach(*new_draw_button);
    Numbered_button* new_hide_button = new Numbered_button{Point{80, 30}, 70, 20, "Hide", cb_hide};
    new_hide_button->set_number(0);
    attach(*new_hide_button);
    Numbered_button* new_rem_button = new Numbered_button{Point{160, 30}, 70, 20, "Remove", cb_rem};
    new_rem_button->set_number(0);
    attach(*new_rem_button);
    new_input.in_box = new_ib;
    new_input.draw_button = new_draw_button;
    new_input.hide_button = new_hide_button;
    new_input.rem_button = new_rem_button;
    enter_menu.push_back(new_input);
}

void Graphix_window::cb_quit(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).quit();
}

void Graphix_window::cb_incr(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).increase_scale();
}

void Graphix_window::cb_decr(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).decrease_scale();
}

void Graphix_window::cb_draw(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbered_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).draw_graph(btn.get_number());
}

void Graphix_window::cb_hide(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbered_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).hide_graph(btn.get_number());
}

void Graphix_window::cb_rem(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbered_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).rem_graph(btn.get_number());
}

void Graphix_window::cb_new(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).new_graph();
}

void Graphix_window::draw_some_graph(std::string str, size_t number)
{
    Segmented_function seg_func(str, scale, center, x_max(), y_max());
    Vector_ref<Function> functions = seg_func.get_segmented_function();
    for (int i = 0; i < functions.size(); i++)
    {
        functions[i].set_color(Graph_lib::Color::black);
        attach(functions[i]);
    }
    if (graphics.size() > number)
        graphics[number] = functions;
    else
        graphics.push_back(functions);
}

void Graphix_window::increase_scale()
{
    if (scale < max_scale)
    {
        detach(*x_axis);
        detach(*y_axis);
        scale = ceil(scale * scale_coef);
        Axis* new_x_axis =
            new Axis{Axis::Orientation::x, Point(x_max() / 2, y_max() / 2), x_max(), scale, "X"};
        new_x_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
        x_axis = new_x_axis;
        attach(*x_axis);
        Axis* new_y_axis =
            new Axis{Axis::Orientation::y, Point(x_max() / 2, y_max() / 2), y_max(), scale, "Y"};
        new_y_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
        y_axis = new_y_axis;
        attach(*y_axis);

        for (size_t i = 0; i < enter_menu.size(); ++i)
            if (!enter_menu[i].is_hidden)
                draw_graph(i);

        button_pushed = true;
    }
}

void Graphix_window::decrease_scale()
{
    if (scale > min_scale)
    {
        detach(*x_axis);
        detach(*y_axis);
        scale = floor(scale / scale_coef);
        Axis* new_x_axis =
            new Axis{Axis::Orientation::x, Point(x_max() / 2, y_max() / 2), x_max(), scale, "X"};
        new_x_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
        x_axis = new_x_axis;
        attach(*x_axis);
        Axis* new_y_axis =
            new Axis{Axis::Orientation::y, Point(x_max() / 2, y_max() / 2), y_max(), scale, "Y"};
        new_y_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
        y_axis = new_y_axis;
        attach(*y_axis);

        for (size_t i = 0; i < enter_menu.size(); ++i)
            if (!enter_menu[i].is_hidden)
                draw_graph(i);

        button_pushed = true;
    }
}

void Graphix_window::draw_graph(size_t i)
{
    hide_graph(i);
    if (graphics.size() > i)
        graphics[i].clear();

    In_box* pin_box = enter_menu[i].in_box;
    func_strings[i] = (*pin_box).get_string();

    if (func_strings[i] != "")
        draw_some_graph(func_strings[i], i);

    enter_menu[i].is_hidden = false;
    button_pushed = true;
}

void Graphix_window::hide_graph(size_t i)
{

    if (graphics.size() > i)
        for (int j = 0; j < graphics[i].size(); ++j)
            detach(graphics[i][j]);
    enter_menu[i].is_hidden = true;
    button_pushed = true;
}

void Graphix_calc::Graphix_window::rem_graph(size_t i)
{
    detach(*(enter_menu[i].in_box));
    detach(*(enter_menu[i].draw_button));
    detach(*(enter_menu[i].hide_button));
    detach(*(enter_menu[i].rem_button));
    if (enter_menu.size() == graphs_number)
        attach(new_button);
    for (size_t j = i + 1; j < enter_menu.size(); ++j)
    {
        enter_menu[j].in_box->move(0, -60);
        enter_menu[j].draw_button->move(0, -60);
        enter_menu[j].hide_button->move(0, -60);
        enter_menu[j].rem_button->move(0, -60);
        enter_menu[j].draw_button->set_number(enter_menu[j].draw_button->get_number() - 1);
        enter_menu[j].hide_button->set_number(enter_menu[j].hide_button->get_number() - 1);
        enter_menu[j].rem_button->set_number(enter_menu[j].rem_button->get_number() - 1);
    }
    enter_menu.erase(enter_menu.begin() + i);
    if (graphics.size() > i)
    {
        for (int j = 0; j < graphics[i].size(); ++j)
            detach(graphics[i][j]);
        graphics.erase(graphics.begin() + i);
    }
    button_pushed = true;
}

void Graphix_window::new_graph()
{
    Input_box new_input;
    In_box* new_ib = new In_box{Point{30, 60 * int(enter_menu.size())}, 200, 30, "y = "};
    attach(*new_ib);
    Numbered_button* new_draw_button =
        new Numbered_button{Point{0, 60 * int(enter_menu.size()) + 30}, 70, 20, "Draw", cb_draw};
    new_draw_button->set_number(int(enter_menu.size()));
    attach(*new_draw_button);
    Numbered_button* new_hide_button =
        new Numbered_button{Point{80, 60 * int(enter_menu.size()) + 30}, 70, 20, "Hide", cb_hide};
    new_hide_button->set_number(int(enter_menu.size()));
    attach(*new_hide_button);
    Numbered_button* new_rem_button =
        new Numbered_button{Point{160, 60 * int(enter_menu.size()) + 30}, 70, 20, "Remove", cb_rem};
    new_rem_button->set_number(int(enter_menu.size()));
    attach(*new_rem_button);
    new_input.in_box = new_ib;
    new_input.draw_button = new_draw_button;
    new_input.hide_button = new_hide_button;
    new_input.rem_button = new_rem_button;
    enter_menu.push_back(new_input);
    Vector_ref<Function> v;
    graphics.push_back(v);
    if (enter_menu.size() == graphs_number)
        detach(new_button);
    button_pushed = true;
}
}  // namespace Graphix_calc