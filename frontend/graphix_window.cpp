// header
#include "graphix_window.h"

// std libs
#include <iostream>

// Graphix_calc
#include "Graphix_calc/segmented_function.h"

// Math_calc
#include "../backend/Math_calc/function_crosses.h"
#include "../backend/Math_calc/function_roots.h"

// servant
#include "servant/constants.h"

using namespace Front_consts;

using Graph_lib::Color;
using Graph_lib::Function;
using Graphix_calc::Segmented_function;

using pix_numb = int;

namespace Frontend {

Graphix_window::Graphix_window(Point left_corner, pix_numb width, pix_numb height,
                               const std::string& title, pix_numb scale)
    : Window(left_corner, width, height, title), scale{scale},
      x_axis{
          new Axis{Axis::Orientation::horisontal, Point(width / 2, height / 2), width, scale, "X"}},
      y_axis{
          new Axis{Axis::Orientation::vertical, Point(width / 2, height / 2), height, scale, "Y"}},
      center{width / 2, height / 2},
      // инициализируем вектор введенных строк (пустыми)
      inputed_strings{"", "", "", "", ""},
      // кнопка quit находится в левом верхнем углу
      quit_button{Point{width - btn_w, 0}, btn_w, btn_h, "Quit", cb_quit},
      // кнопки изменения масштаба находятся справа и являются квадратами
      incr_button{Point{width - scl_btn_side, btn_h}, scl_btn_side, scl_btn_side, "+", cb_incr},
      decr_button{Point{width - scl_btn_side, btn_h + scl_btn_side}, scl_btn_side, scl_btn_side,
                  "-", cb_decr},
      // а кнопка new_graph находится правее меню
      new_graph_button{Point{inp_box_w + scl_btn_side, 0}, btn_w, btn_h, "New graph", cb_new},
      // кнопки для точек
      sh_points{Point{width - btn_w, height - 2 * btn_h}, btn_w, btn_h, "Points", cb_show_points},
      hd_points{Point{width - btn_w, height - btn_h}, btn_w, btn_h, "Hide", cb_hide_points}
{
    // не даём пользователю менять размеры окна
    size_range(width, height, width, height);

    // задаём цвет осям
    x_axis->set_color(Color::Color_type::dark_cyan);
    y_axis->set_color(Color::Color_type::dark_cyan);

    // поскольку enter_menu не была инициализирована, её size() = 0, а значит, мы создаем нулевой по
    // счету input_box
    Input_box* new_graph = new Input_box{short(enter_menu.size()), cb_draw, cb_hide, cb_rem};
    new_graph->set_number(enter_menu.size());
    // и добавляем его в общий список всех input_box'ов
    enter_menu.push_back(new_graph);

    // аттачим всё
    attach(*x_axis);
    attach(*y_axis);
    attach(quit_button);
    attach(incr_button);
    attach(decr_button);
    attach(new_graph_button);
    attach(sh_points);
    attach(hd_points);
    new_graph->attach(*this);
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

void Graphix_window::change_scale(pix_numb new_scale)
{
    if (new_scale < max_scale && new_scale > min_scale)
    {
        scale = new_scale;

        // детачим оси и высвобождаем память
        detach(*x_axis);
        detach(*y_axis);
        delete x_axis;
        delete y_axis;

        // создаём новые оси и задаём цвет
        x_axis = new Axis{Axis::Orientation::horisontal, center, x_max(), scale, "X"};
        x_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);

        y_axis = new Axis{Axis::Orientation::vertical, center, y_max(), scale, "Y"};
        y_axis->set_color(Graph_lib::Color::Color_type::dark_cyan);

        // аттачим новые оси
        attach(*x_axis);
        attach(*y_axis);

        // перересовываем все графики
        for (size_t i = 0; i < enter_menu.size(); ++i)
            if (!enter_menu[i]->is_hidden())
                draw_graph(i);

        // перерисовываем все точки, если они видны
        if (is_points_visible)
            show_points();

        button_pushed = true;
    }
}

void Graphix_window::increase_scale() { change_scale(scale * scale_coef); }

void Graphix_window::decrease_scale() { change_scale(scale / scale_coef); }

void Graphix_window::draw_graph(size_t graph_number)
{
    // чистим старый график и точки
    graph_clear(graph_number);

    // записываем в вектор введенных строк то, что ввёл
    inputed_strings[graph_number] = enter_menu[graph_number]->get_string();

    // создаём сегментированную функцию
    Segmented_function seg_func(inputed_strings[graph_number], scale, center, x_max(), y_max());
    Vector_ref<Function> graphic = seg_func.get_segmented_function();

    // аттачим сегментированную функцию
    for (int i = 0; i < graphic.size(); i++)
    {
        graphic[i].set_color(Color::black);
        attach(graphic[i]);
    }

    // записываем новую функцию в общий массив всех графиков
    if (graphics.size() > graph_number)
        graphics[graph_number] = graphic;
    else
        graphics.push_back(graphic);

    // выводим наше окно
    enter_menu[graph_number]->show();

    button_pushed = true;
}

void Graphix_window::hide_graph(size_t graph_number)
{
    // прячем старый график и чистим память
    graph_clear(graph_number);

    button_pushed = true;
}

void Graphix_window::rem_graph(size_t graph_number)
{
    // детачим кнопку под переданным номером
    enter_menu[graph_number]->detach(*this);

    // возвращаем кнопку "new_graph", если был удален последний
    if (enter_menu.size() == graphs_max_amount)
        attach(new_graph_button);

    // проходимся по всем инпут_боксам начиная со следующего
    for (size_t j = graph_number + 1; j < enter_menu.size(); ++j)
    {
        // двигаем их вверх и меняем номер на пердыдущий
        enter_menu[j]->move(0, -inp_box_indent);
        enter_menu[j]->set_number(enter_menu[j]->get_number() - 1);
    }
    // также изменяем размеры самого вектора
    enter_menu.erase(enter_menu.begin() + graph_number);

    if (graphics.size() > graph_number)
    {
        // детачим этот график и чистим память
        graph_clear(graph_number);
        // также изменяем размеры самого вектора
        graphics.erase(graphics.begin() + graph_number);
    }

    // в случае удаления одного из графиков прячем все точки
    hide_points();

    button_pushed = true;
}

void Graphix_window::graph_clear(size_t graph_number)
{
    // убираем все спрятанные фрагменты графика с экрана
    if (graphics.size() > graph_number)
    {
        for (int j = 0; j < graphics[graph_number].size(); ++j)
            detach(graphics[graph_number][j]);
    }

    // прячем окно
    enter_menu[graph_number]->hide();

    // высвобождаем память
    if (graphics.size() > graph_number)
        graphics[graph_number].clear();
}

void Graphix_window::new_graph()
{
    // создаем новый инпут_бокс
    Input_box* new_graph = new Input_box{short(enter_menu.size()), cb_draw, cb_hide, cb_rem};

    // задаём ему последний номер и аттачим его
    new_graph->set_number(enter_menu.size());
    new_graph->attach(*this);

    // добавляем новый инпут_бокс в соотв. вектор
    enter_menu.push_back(new_graph);

    // увеличиваем размер вектора с графиками (резервируем под новую сегментированную функцию)
    graphics.push_back(Vector_ref<Function>{});

    // если количество графиков стало максимально, скрываем кнопку "new_graph"
    if (enter_menu.size() == graphs_max_amount)
        detach(new_graph_button);

    button_pushed = true;
}

void Graphix_window::show_points()
{
    clear_points();
    is_points_visible = true;

    auto convert_to_pix = [&] (Math_calc::Point p) -> Graph_lib::Point {
        return Graph_lib::Point{center.x + int(p.x * scale), center.y - int(p.y * scale)};
    };

    for (auto& input_box : enter_menu)
    {
        string str = input_box->get_string();
        if (!str.empty())
        {
            Math_calc::function_roots fr{
                str, -((double)x_max() / (2 * scale)), ((double)x_max() / (2 * scale)),
                (double)y_max() / (2 * scale), (((double)x_max() / (scale * 100 * 2.5)))};
            Graph_lib::Marks* dots = new Graph_lib::Marks{"x"};
            for (auto& p : fr.get_function_roots())
                dots->add(convert_to_pix(p));
            attach(*dots);
            all_points.push_back(*dots);
        }
    }

    for (size_t i = 0; i < enter_menu.size(); i++)
    {
        for (size_t j = i + 1; j < enter_menu.size(); j++)
        {

            if (!enter_menu[i]->get_string().empty() && !enter_menu[j]->get_string().empty())
            {
                Math_calc::function_crosses fc{
                    {enter_menu[i]->get_string(), enter_menu[j]->get_string()},
                    -((double)x_max() / (2 * scale)),
                    ((double)x_max() / (2 * scale)),
                    (double)y_max() / (2 * scale),
                    (((double)x_max() / (scale * 100 * 2.5)))};
                Graph_lib::Marks* dots = new Graph_lib::Marks{"o"};
                for (auto& p : fc.get_functions_crosses())
                    dots->add(convert_to_pix(p));
                attach(*dots);
                all_points.push_back(*dots);
            }
        }
    }

    button_pushed = true;
}

void Graphix_window::hide_points()
{
    clear_points();
    is_points_visible = false;
    button_pushed = true;
}

void Graphix_window::clear_points()
{
    for (int i = 0; i < all_points.size(); i++)
        detach(all_points[i]);

    all_points.clear();
}

}  // namespace Frontend