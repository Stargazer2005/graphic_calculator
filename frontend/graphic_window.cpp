#include "graphic_window.h"
#include "../backend/backend.h"
#include <Graph_lib/Simple_window.h>

using namespace Graph_lib;

Graphic_lib::Graphic_window::Graphic_window(Point xy, pix_numb w, pix_numb h,
                                            const std::string& title, pix_numb scale)
    : Window(xy, w, h, title), scale{scale},
      x_axis{new Graphic_lib::Axis{Graphic_lib::Axis::Orientation::x,
                                   Point(x_max() / 2, y_max() / 2), x_max(), scale, "X"}},
      y_axis{new Graphic_lib::Axis{Graphic_lib::Axis::Orientation::y,
                                   Point(x_max() / 2, y_max() / 2), y_max(), scale, "Y"}},
      center{w / 2, h / 2}, quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit},
      zoom_menu{Point{w - 30, 20}, 30, 30, Graph_lib::Menu::Kind::vertical, ""},
      incr_button{Point{0, 0}, 30, 30, "+", cb_incr},
      decr_button{Point{0, 0}, 30, 30, "-", cb_decr},
      draw_button{Point{230, 0}, 70, 20, "Draw", cb_draw},
      new_button{Point{230, 20}, 70, 20, "New graph", cb_new}
{
    x_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
    attach(*x_axis);

    y_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);
    attach(*y_axis);

    attach(quit_button);
    zoom_menu.attach(incr_button);
    zoom_menu.attach(decr_button);
    attach(zoom_menu);
    attach(draw_button);
    attach(new_button);
    for (short i = 0; i < graphs_number; ++i)
        func_strings.push_back("");
    In_box* new_ib = new In_box{Point{30, 0}, 200, 30, "y = "};
    enter_menu.push_back(*new_ib);
    attach(*new_ib);
}

void Graphic_lib::Graphic_window::cb_quit(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphic_window&>(btn.window()).quit();
}

void Graphic_lib::Graphic_window::cb_incr(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphic_window&>(btn.window()).increase_scale();
}

void Graphic_lib::Graphic_window::cb_decr(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphic_window&>(btn.window()).decrease_scale();
}

void Graphic_lib::Graphic_window::cb_draw(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphic_window&>(btn.window()).draw_graphs();
}

void Graphic_lib::Graphic_window::cb_new(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphic_window&>(btn.window()).new_graph();
}

void Graphic_lib::Graphic_window::draw_some_graph(std::string str)
{
    Vector_ref<Function> functions =
        Backend::segmented_function(str, scale, center, x_max(), y_max());
    for (int i = 0; i < functions.size(); i++)
        attach(functions[i]);
    graphics.push_back(functions);
}

void Graphic_lib::Graphic_window::increase_scale()
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
        draw_graphs();
    }
}

void Graphic_lib::Graphic_window::decrease_scale()
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
        draw_graphs();
    }
}

void Graphic_lib::Graphic_window::draw_graphs()
{
    for (size_t i = 0; i < graphics.size(); ++i)
        for (int j = 0; j < graphics[i].size(); ++j)
            detach(graphics[i][j]);

    if (graphics.size() > 0)
        graphics.clear();  // очищает память тоже
    for (int i = 0; i < enter_menu.size(); ++i)
    {

        func_strings[i] = enter_menu[i].get_string();

        if (func_strings[i] != "")
        {
            draw_some_graph(func_strings[i]);
        }
    }
    button_pushed = true;
}

void Graphic_lib::Graphic_window::new_graph()
{
    In_box* new_ib = new In_box{Point{30, 30 * enter_menu.size()}, 200, 30, "y = "};
    enter_menu.push_back(*new_ib);
    attach(*new_ib);
    if (enter_menu.size() == graphs_number)
        detach(new_button);
    button_pushed = true;
}
