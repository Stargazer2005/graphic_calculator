#include <Graph_lib/Window.h>

#include "graphix_window.h"
#include "segmented_function.h"

#include "servant/constants.h"

using pix_numb = int;

using namespace Graph_lib;
using namespace Front_consts;
using Graphix_calc::Button_num;

namespace Graphix_calc {

Graphix_window::Graphix_window(Point xy, pix_numb w, pix_numb h, const std::string& title,
                               pix_numb scale)
    : Window(xy, w, h, title), scale{scale},
      x_axis{new Graphix_calc::Axis{Graphix_calc::Axis::Orientation::x,
                                    Point(x_max() / 2, y_max() / 2), x_max(), scale, "X"}},
      y_axis{new Graphix_calc::Axis{Graphix_calc::Axis::Orientation::y,
                                    Point(x_max() / 2, y_max() / 2), y_max(), scale, "Y"}},
      center{w / 2, h / 2}, quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit},
      zoom_menu{Point{w - 30, 20}, 30, 30, Graph_lib::Menu::Kind::vertical, ""},
      incr_button{Point{0, 0}, 30, 30, "+", cb_incr},
      decr_button{Point{0, 0}, 30, 30, "-", cb_decr},
      new_button{Point{230, 0}, 70, 20, "New graph", cb_new}
{
    x_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
    attach(*x_axis);

    y_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
    attach(*y_axis);

    attach(quit_button);
    zoom_menu.attach(incr_button);
    zoom_menu.attach(decr_button);
    attach(zoom_menu);
    attach(new_button);

    for (short i = 0; i < graphs_number; ++i)
        func_strings.push_back("");

    Input_box new_input;
    In_box* new_ib = new In_box{Point{30, 0}, 200, 30, "y = "};
    attach(*new_ib);
    Button_num* new_draw_button = new Button_num{Point{0, 30}, 70, 20, "Draw", cb_draw};
    new_draw_button->number = 0;
    attach(*new_draw_button);
    Button_num* new_hide_button = new Button_num{Point{80, 30}, 70, 20, "Hide", cb_hide};
    new_hide_button->number = 0;
    attach(*new_hide_button);
    new_input.in_box = new_ib;
    new_input.draw_button = new_draw_button;
    new_input.hide_button = new_hide_button;
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
    auto& btn = Graph_lib::reference_to<Button_num>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).draw_graph(btn.number);
}

void Graphix_window::cb_hide(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button_num>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).hide_graph(btn.number);
}

void Graphix_window::cb_new(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).new_graph();
}

// void Graphix_window::cb_del(void*, void* widget)
// {
//   auto& btn = Graph_lib::reference_to<Button>(widget);
//   dynamic_cast<Graphix_window&>(btn.window()).del_graph();
// }

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

void Graphix_window::new_graph()
{
    Input_box new_input;
    In_box* new_ib = new In_box{Point{30, 60 * int(enter_menu.size())}, 200, 30, "y = "};
    attach(*new_ib);
    Button_num* new_draw_button =
        new Button_num{Point{0, 60 * int(enter_menu.size()) + 30}, 70, 20, "Draw", cb_draw};
    new_draw_button->number = int(enter_menu.size());
    attach(*new_draw_button);
    Button_num* new_hide_button =
        new Button_num{Point{80, 60 * int(enter_menu.size()) + 30}, 70, 20, "Hide", cb_hide};
    new_hide_button->number = int(enter_menu.size());
    attach(*new_hide_button);
    new_input.in_box = new_ib;
    new_input.draw_button = new_draw_button;
    new_input.hide_button = new_hide_button;
    enter_menu.push_back(new_input);
    Vector_ref<Function> v;
    graphics.push_back(v);
    // if (enter_menu.size() == 2)
    //   attach(del_button);
    if (enter_menu.size() == graphs_number)
        detach(new_button);
    button_pushed = true;
}

// void Graphix_calc::Graphix_window::del_graph()
// {
//   detach(*(enter_menu[-1].in_box));
//   detach(*(enter_menu[-1].draw_button));
//   detach(*(enter_menu[-1].hide_button));
//   delete enter_menu[-1].in_box;
//   delete enter_menu[-1].draw_button;
//   delete enter_menu[-1].hide_button;
//   enter_menu.pop_back();
//   if (graphics.size() > 0)
//     graphics.pop_back();
//   if (enter_menu.size() == 1)
//     detach(del_button);
//   button_pushed = true;
// }
}  // namespace Graphix_calc