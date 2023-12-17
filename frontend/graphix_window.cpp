// header
#include "Graphix_window.h"

// servant
#include "servant/constants.h"
using namespace Front_consts;

namespace Frontend {

// из-за количества кнопок на экране конструктор сильно перегружен, но что поделать :)

Graphix_window::Graphix_window(Graph_lib::Point left_corner, int width, int height,
                               const string& title, int scale)
    : Graph_lib::Window(left_corner, width, height, title), scale{scale},
      // точка начала системы координат смещена вправо, чтобы графики и оси не заезжали на меню
      origin{(width + function_box_w) / 2, height / 2},
      x_axis{new Axis{Axis::Orientation::horisontal, origin, width - function_box_w, scale, "X"}},
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
      point_box{width, height, cb_show_points, cb_hide_points}
{
    // не даём пользователю менять размеры окна
    size_range(width, height, width, height);

    // задаём цвет окну
    // this->color(Color::white);

    // инициализируем вектор введенных строк (пустыми)
    for (size_t i = 0; i < max_functions_amount; i++)
        inputed_funcs.push_back(empty_str);

    // задаём цвет осям
    x_axis->set_color(Color::Color_type::dark_cyan);
    y_axis->set_color(Color::Color_type::dark_cyan);

    // поскольку enter_menu не была инициализирована, её size() = 0, а значит, мы создаем нулевой по
    // счету Function_box
    Function_box* cb_new_func = new Function_box{enter_menu.size(), cb_draw_graph, cb_hide_graph,
                                                 cb_rem_func,       cb_draw_der,   cb_hide_der};
    cb_new_func->set_number(enter_menu.size());
    // и добавляем его в общий список всех input_box'ов
    enter_menu.push_back(cb_new_func);

    // аттачим всё
    attach(*x_axis);
    attach(*y_axis);
    attach(quit_button);
    attach(incr_button);
    attach(decr_button);
    attach(new_button);

    cb_new_func->attach(*this);
    point_box.attach(*this);
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
    dynamic_cast<Graphix_window&>(btn.window()).draw_graph(btn.get_number());
}

void Graphix_window::cb_hide_graph(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).hide_graph(btn.get_number());
}

void Graphix_window::cb_draw_der(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).draw_der(btn.get_number());
}

void Graphix_window::cb_hide_der(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).hide_der(btn.get_number());
}

void Graphix_window::cb_quit(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).quit();
}

void Graphix_window::cb_rem_func(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Numbed_button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).rem_func(btn.get_number());
}

void Graphix_window::cb_new_func(void*, void* widget)
{
    auto& btn = Graph_lib::reference_to<Button>(widget);
    dynamic_cast<Graphix_window&>(btn.window()).cb_new_func();
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

void Graphix_window::change_scale(int new_scale)
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

        x_axis =
            new Axis{Axis::Orientation::horisontal, origin, win_w() - function_box_w, scale, "X"};
        x_axis->set_color(Color::Color_type::dark_cyan);

        y_axis = new Axis{Axis::Orientation::vertical, origin, win_h(), scale, "Y"};
        y_axis->set_color(Color::Color_type::dark_cyan);

        // аттачим новые оси
        attach(*x_axis);
        attach(*y_axis);

        // перересовываем все графики
        for (size_t i = 0; i < enter_menu.size(); ++i)
            if (!enter_menu[i]->is_graph_hidden())
                draw_graph(i);

        // перересовываем все производные
        for (size_t i = 0; i < enter_menu.size(); ++i)
            if (!enter_menu[i]->is_der_hidden())
                draw_der(i);

        // перерисовываем все точки, если они видны
        if (is_points_visible)
            show_points();
    }
}

void Graphix_window::incr_scale()
{
    change_scale(scale * scale_coef);
    button_pushed = true;
}

void Graphix_window::decr_scale()
{
    change_scale(scale / scale_coef);
    button_pushed = true;
}

void Graphix_window::draw_graph(size_t func_index)
{
    // чистим старый график и точки
    clear_graph(func_index);

    // записываем в вектор введенных строк то, что ввёл пользователь
    fill_graphs();

    // добавили обработку ошибок
    try
    {
        // локальная переменная - введенная строка
        string func = inputed_funcs[func_index];

        // раскрываем все зависимости вида y_n
        Backend::expose_dep_function(inputed_funcs, func, func_index);

        // создаём сегментированную функцию
        Segmented_function seged_func(func, scale, origin, win_w(), win_h());
        // график - композиция нескольких графиков, каждый из которых определен на своём сегменте
        Vector_ref<Function> graphic = seged_func.get_segmented_function();

        //  аттачим сегментированную функцию
        for (int i = 0; i < graphic.size(); i++)
        {
            graphic[i].set_color(Color::black);
            attach(graphic[i]);
        }

        // записываем новую функцию в общий массив всех графиков
        if (graphics.size() > func_index)
            graphics[func_index] = graphic;
        else
            graphics.push_back(graphic);

        // чистим поле с ошибкой и выводим график
        enter_menu[func_index]->set_message(empty_str);
        enter_menu[func_index]->graph_show();
    }
    catch (const std::exception& e)
    {
        // выводим ошибку и прячем график
        enter_menu[func_index]->set_message(e.what());
        enter_menu[func_index]->graph_hide();
    }

    button_pushed = true;
}

void Graphix_window::draw_der(size_t der_index)
{
    // чистим старый график и точки
    clear_der(der_index);

    // записываем в вектор введенных строк то, что ввёл пользователь
    fill_graphs();

    // добавили обработку ошибок
    try
    {
        // локальная переменная - введенная строка
        string func = inputed_funcs[der_index];

        // раскрываем все зависимости вида y_n
        Backend::expose_dep_function(inputed_funcs, func, der_index);

        // создаём сегментированную функцию
        Segmented_function seged_func(func, scale, origin, win_w(), win_h());
        Vector_ref<Function> derivative = seged_func.get_segmented_function();

        // аттачим сегментированную функцию
        for (int i = 0; i < derivative.size(); i++)
        {
            derivative[i].set_color(Color::dark_red);
            attach(derivative[i]);
        }

        // записываем новую функцию в общий массив всех графиков
        if (derivatives.size() > der_index)
            derivatives[der_index] = derivative;
        else
            derivatives.push_back(derivative);

        // чистим поле с ошибкой и выводим график
        enter_menu[der_index]->set_message(empty_str);
        enter_menu[der_index]->der_show();
    }
    catch (const std::exception& e)
    {
        // выводим ошибку и прячем график
        enter_menu[der_index]->set_message(e.what());
        enter_menu[der_index]->der_hide();
    }

    button_pushed = true;
}

void Graphix_window::hide_graph(size_t func_index)
{
    // прячем старый график и чистим память
    clear_graph(func_index);

    button_pushed = true;
}

void Graphix_window::hide_der(size_t der_index)
{
    // прячем старый график и чистим память
    clear_der(der_index);

    button_pushed = true;
}

void Graphix_window::rem_func(size_t func_index)
{
    // детачим кнопку под переданным номером
    enter_menu[func_index]->detach(*this);

    // проходимся по всем инпут_боксам начиная со следующего
    for (size_t j = func_index + 1; j < enter_menu.size(); ++j)
    {
        // двигаем их вверх и меняем номер на пердыдущий
        enter_menu[j]->move(0, -function_box_h);
        enter_menu[j]->set_number(enter_menu[j]->get_number() - 1);
    }
    // также изменяем размеры самого вектора
    enter_menu.erase(enter_menu.begin() + func_index);

    if (graphics.size() > func_index)
    {
        // детачим этот график и чистим память
        clear_graph(func_index);
        // также изменяем размеры самого вектора
        graphics.erase(graphics.begin() + func_index);
    }

    // возвращаем кнопку "cb_new_func" на экран, если был удален хоть один
    if (enter_menu.size() == max_functions_amount - 1)
        attach(new_button);

    // если не осталось полей для ввода, двигаем "new_button"
    if (enter_menu.empty())
        new_button.move(-function_box_w, 0);

    // в случае удаления одного из графиков удаляем все точки
    clear_points();

    button_pushed = true;
}

void Graphix_window::clear_graph(size_t func_index)
{
    // убираем все спрятанные фрагменты графика с экрана
    if (graphics.size() > func_index)
    {
        for (int j = 0; j < graphics[func_index].size(); ++j)
            detach(graphics[func_index][j]);
    }

    // прячем график
    enter_menu[func_index]->graph_hide();

    // высвобождаем память
    if (graphics.size() > func_index)
        graphics[func_index].clear();
}

void Graphix_window::clear_der(size_t der_index)
{
    // убираем все спрятанные фрагменты графика с экрана
    if (derivatives.size() > der_index)
    {
        for (int j = 0; j < derivatives[der_index].size(); ++j)
            detach(derivatives[der_index][j]);
    }

    // прячем график
    enter_menu[der_index]->der_hide();

    // высвобождаем память
    if (derivatives.size() > der_index)
        derivatives[der_index].clear();
}

void Graphix_window::cb_new_func()
{
    // создаем новый инпут_бокс
    Function_box* cb_new_func = new Function_box{enter_menu.size(), cb_draw_graph, cb_hide_graph,
                                                 cb_rem_func,       cb_draw_der,   cb_hide_der};

    // задаём ему последний номер и аттачим его
    cb_new_func->set_number(enter_menu.size());
    cb_new_func->attach(*this);

    // если до увеличения массив инпут_боксов был пуст, то мы обратно двигаем new_button
    if (enter_menu.empty())
        new_button.move(function_box_w, 0);

    // добавляем новый инпут_бокс в соотв. вектор
    enter_menu.push_back(cb_new_func);

    // увеличиваем размер вектора с графиками (резервируем под новую сегментированную функцию)
    graphics.push_back(Vector_ref<Function>{});

    // если количество графиков стало максимально, скрываем кнопку "cb_new_func"
    if (enter_menu.size() == max_functions_amount)
        detach(new_button);

    button_pushed = true;
}

void Graphix_window::show_points()
{
    // чистим память от всех предыдущих точек
    clear_points();

    is_points_visible = true;

    // воспомогательная функция, которая переводит бэкендовы вещественные точки в пиксельные
    auto convert_to_pix = [&] (Math_calc::Point p) -> Graph_lib::Point
    {
        return Graph_lib::Point{origin.x + static_cast<int>(p.x * scale),
                                origin.y - static_cast<int>(p.y * scale)};
    };

    // переводим границы экрана в вещественные, чтобы использовать для бэкендовских функций

    double l_border = -((double)win_w() / (2 * scale)) + function_box_w / (2 * scale);
    double r_border = -l_border;
    double h_border = (double)win_h() / (2 * scale);
    double point_prec = (((double)win_w() / (scale * 2500)));

    // проходимся по всем строкам, куда пользователь вводит функции и рисуем их корни,
    // (если они не скрыты)
    for (auto& input_box : enter_menu)
    {
        string func = input_box->get_string();
        size_t func_index = input_box->get_number();
        if (!input_box->is_graph_hidden())
        {
            if (!func.empty())
            {
                // локальная переменная - введенная строка
                string func = inputed_funcs[func_index];
                // раскрываем все зависимости вида y_n
                Backend::expose_dep_function(inputed_funcs, func, func_index);
                Math_calc::function_roots fr{func, l_border, r_border, h_border, point_prec};
                // создаём марки, добавляем их на окно
                Marks* dots = new Marks{"x"};
                for (auto& p : fr.get_function_roots())
                    dots->add(convert_to_pix(p));
                attach(*dots);
                // и добавляем в общий массив всех точек на экране
                all_points.push_back(*dots);
            }
        }
    }

    // проходимся по всем строкам, куда пользователь вводит функции и рисуем их экстремумы,
    // (если они не скрыты)
    for (auto& input_box : enter_menu)
    {
        string func = input_box->get_string();
        size_t func_index = input_box->get_number();
        if (!input_box->is_graph_hidden())
        {
            if (!func.empty())
            {
                // локальная переменная - введенная строка
                string func = inputed_funcs[func_index];

                // раскрываем все зависимости вида y_n
                Backend::expose_dep_function(inputed_funcs, func, func_index);
                Math_calc::function_extremes fe{func, l_border, r_border, h_border, point_prec};
                // создаём марки, добавляем их на окно
                Marks* dots = new Marks{"#"};
                for (auto& p : fe.get_function_extremes())
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

            auto& input_box = enter_menu[i];
            string func = input_box->get_string();
            // "другой input_box"
            auto& oth_input_box = enter_menu[j];
            string oth_func = oth_input_box->get_string();

            bool hidden = input_box->is_graph_hidden() || oth_input_box->is_graph_hidden();
            bool empty = func.empty() || oth_func.empty();

            if (!hidden)
            {
                if (!empty)
                {
                    // раскрываем все зависимости вида y_n для func
                    Backend::expose_dep_function(inputed_funcs, func, i);
                    // раскрываем все зависимости вида y_n для oth_func
                    Backend::expose_dep_function(inputed_funcs, oth_func, j);

                    Math_calc::function_crosses fc{
                        {func, oth_func}, l_border, r_border, h_border, point_prec};
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