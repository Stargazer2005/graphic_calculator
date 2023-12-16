// header
#include "graphix_window.h"
#include <iostream>
// servant
#include "servant/constants.h"
using namespace Front_consts;

using pix_numb = int;

namespace Frontend {

// из-за количества кнопок на экране конструктор сильно перегружен, но что поделать :)
Graphix_window::Graphix_window(Graph_lib::Point left_corner, pix_numb width, pix_numb height,
                               const string& title, pix_numb scale)
    : Graph_lib::Window(left_corner, width, height, title), scale{scale},
      x_axis{new Axis{Axis::Orientation::horisontal, Graph_lib::Point(width / 2, height / 2), width,
                      scale, "X"}},
      y_axis{new Axis{Axis::Orientation::vertical, Graph_lib::Point(width / 2, height / 2), height,
                      scale, "Y"}},
      center{width / 2, height / 2},
      // кнопка quit находится в левом верхнем углу
      quit_button{Graph_lib::Point{width - btn_w, 0}, btn_w, btn_h, "Quit", cb_quit},
      // кнопки изменения масштаба находятся справа и являются квадратами
      incr_button{Graph_lib::Point{width - scl_btn_side, btn_h}, scl_btn_side, scl_btn_side, "+",
                  cb_incr},
      decr_button{Graph_lib::Point{width - scl_btn_side, btn_h + scl_btn_side}, scl_btn_side,
                  scl_btn_side, "-", cb_decr},
      // а кнопка new_graph находится правее меню
      new_graph_button{Graph_lib::Point{inp_box_w + scl_btn_side, 0}, btn_w, btn_h, "New graph",
                       cb_new},
      // меню с точками
      point_box{width, height, cb_show_points, cb_hide_points}
{

    // инициализируем вектор введенных строк (пустыми)
    for (size_t i = 0; i < graphs_max_amount; i++)
        inputed_funcs.push_back(empty_str);

    // не даём пользователю менять размеры окна
    size_range(width, height, width, height);

    // задаём цвет окну
    // this->color(Color::white);

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

    new_graph->attach(*this);
    point_box.attach(*this);
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
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).draw_graph(btn.get_number());
}

void Graphix_window::cb_hide(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).hide_graph(btn.get_number());
}

void Graphix_window::cb_rem(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
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
        x_axis->set_color(Color::Color_type::dark_cyan);

        y_axis = new Axis{Axis::Orientation::vertical, center, y_max(), scale, "Y"};
        y_axis->set_color(Color::Color_type::dark_cyan);

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
    }
}

void Graphix_window::increase_scale()
{
    change_scale(scale * scale_coef);
    button_pushed = true;
}

void Graphix_window::decrease_scale()
{
    change_scale(scale / scale_coef);
    button_pushed = true;
}

void Graphix_window::draw_graph(size_t graph_index)
{
    // чистим старый график и точки
    clear_graph(graph_index);

    // записываем в вектор введенных строк то, что ввёл пользователь
    fill_graphs();
    std::cout << "fill_used" << std::endl;
    for (size_t i = 0; i < inputed_funcs.size(); i++)
    {
        std::cout << i << " " << inputed_funcs[i] << std::endl;
    }
    //   return;

    // добавили обработку ошибок
    try
    {
        string exposed_func =
            Backend::exposed_dep_function(inputed_funcs, inputed_funcs[graph_index], graph_index);

        // создаём сегментированную функцию
        Segmented_function seg_func(exposed_func, scale, center, x_max(), y_max());
        Vector_ref<Function> graphic = seg_func.get_segmented_function();

        // аттачим сегментированную функцию
        for (int i = 0; i < graphic.size(); i++)
        {
            graphic[i].set_color(Color::black);
            attach(graphic[i]);
        }

        // записываем новую функцию в общий массив всех графиков
        if (graphics.size() > graph_index)
            graphics[graph_index] = graphic;
        else
            graphics.push_back(graphic);

        // чистим поле с ошибкой и выводим график
        enter_menu[graph_index]->set_message(empty_str);
        enter_menu[graph_index]->graph_show();
    }
    catch (const std::exception& e)
    {
        // выводим ошибку и прячем график
        enter_menu[graph_index]->set_message(e.what());
        enter_menu[graph_index]->graph_hide();
    }

    button_pushed = true;
}

void Graphix_window::hide_graph(size_t graph_index)
{
    // прячем старый график и чистим память
    clear_graph(graph_index);

    button_pushed = true;
}

void Graphix_window::rem_graph(size_t graph_index)
{
    // детачим кнопку под переданным номером
    enter_menu[graph_index]->detach(*this);

    // проходимся по всем инпут_боксам начиная со следующего
    for (size_t i = graph_index + 1; i < enter_menu.size(); i++)
    {
        // двигаем их вверх и меняем номер на пердыдущий
        enter_menu[i]->move(0, -inp_box_indent_h);
        enter_menu[i]->set_number(enter_menu[i]->get_number() - 1);
    }
    // также изменяем размеры самого вектора
    enter_menu.erase(enter_menu.begin() + graph_index);

    if (graphics.size() > graph_index)
    {
        // детачим этот график и чистим память
        clear_graph(graph_index);
        // также изменяем размеры самого вектора
        graphics.erase(graphics.begin() + graph_index);
    }

    // возвращаем кнопку "new_graph" на экран, если был удален хоть один
    if (enter_menu.size() == graphs_max_amount - 1)
        attach(new_graph_button);

    // если не осталось полей для ввода, двигаем "new_graph_button"
    if (enter_menu.empty())
        new_graph_button.move(-inp_box_all_w, 0);

    // в случае удаления одного из графиков удаляем все точки
    clear_points();

    button_pushed = true;
}

void Graphix_window::clear_graph(size_t graph_index)
{
    // убираем все спрятанные фрагменты графика с экрана
    if (graphics.size() > graph_index)
    {
        for (int i = 0; i < graphics[graph_index].size(); ++i)
            detach(graphics[graph_index][i]);
    }

    // прячем график
    enter_menu[graph_index]->graph_hide();

    // высвобождаем память
    if (graphics.size() > graph_index)
        graphics[graph_index].clear();
}

void Graphix_window::new_graph()
{
    // создаем новый инпут_бокс
    Input_box* new_graph = new Input_box{short(enter_menu.size()), cb_draw, cb_hide, cb_rem};

    // задаём ему последний номер и аттачим его
    new_graph->set_number(enter_menu.size());
    new_graph->attach(*this);

    // если до увеличения массив инпут_боксов был пуст, то мы обратно двигаем new_graph_button
    if (enter_menu.empty())
        new_graph_button.move(inp_box_all_w, 0);

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
    // чистим память от всех предыдущих точек
    clear_points();

    is_points_visible = true;

    // воспомогательная функция, которая переводит бэкендовы вещественные точки в пиксельные
    auto convert_to_pix = [&] (Math_calc::Point p) -> Graph_lib::Point {
        return Graph_lib::Point{center.x + int(p.x * scale), center.y - int(p.y * scale)};
    };

    // переводим границы экрана в вещественные, чтобы использовать для бэкендовских функций

    double l_border = -((double)x_max() / (2 * scale));
    double r_border = -l_border;
    double h_border = (double)y_max() / (2 * scale);
    double point_prec = (((double)x_max() / (scale * 2500)));

    // проходимся по всем строкам, куда пользователь вводит функции и рисуем их корни,
    // (если они не скрыты)
    for (const auto& input_box : enter_menu)
    {
        string func = input_box->get_string();
        size_t graph_index = input_box->get_number();
        if (!input_box->is_hidden())
        {
            if (!func.empty())
            {
                string exposed_func =
                    Backend::exposed_dep_function(inputed_funcs, func, graph_index);
                Math_calc::function_roots fr{exposed_func, l_border, r_border, h_border,
                                             point_prec};
                // создаём марки, добавляем их на окно
                Marks* dots = new Marks{"x"};
                for (const auto& p : fr.get_function_roots())
                    dots->add(convert_to_pix(p));
                attach(*dots);
                // и добавляем в общий массив всех точек на экране
                all_points.push_back(*dots);
            }
        }
    }

    // проходимся по всем строкам, куда пользователь вводит функции и их всевозможные пересечения
    // (если они не скрыты)
    for (size_t i = 0; i < enter_menu.size(); i++)
    {
        for (size_t j = i + 1; j < enter_menu.size(); j++)
        {

            const auto& input_box = enter_menu[i];
            string func = input_box->get_string();
            // "другой input_box"
            const auto& oth_input_box = enter_menu[j];
            string oth_func = oth_input_box->get_string();

            bool hidden = input_box->is_hidden() || oth_input_box->is_hidden();
            bool empty = func.empty() || oth_func.empty();

            if (!hidden)
            {
                if (!empty)
                {
                    string exposed_func = Backend::exposed_dep_function(inputed_funcs, func, i);
                    string oth_exposed_func =
                        Backend::exposed_dep_function(inputed_funcs, oth_func, j);

                    Math_calc::function_crosses fc{
                        {exposed_func, oth_exposed_func}, l_border, r_border, h_border, point_prec};
                    // создаём марки, добавляем их на окно
                    Marks* dots = new Marks{"o"};
                    for (auto& p : fc.get_functions_crosses())
                        dots->add(convert_to_pix(p));
                    attach(*dots);
                    // и добавляем в общий массив всех точек на экране
                    all_points.push_back(*dots);
                }
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

void Graphix_window::fill_graphs()
{
    for (size_t i = 0; i < enter_menu.size(); i++)
        inputed_funcs[i] = enter_menu[i]->get_string();
}

}  // namespace Frontend