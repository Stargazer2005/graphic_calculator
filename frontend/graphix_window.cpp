// header
#include "Graphix_window.h"

// std libs
// #include <iostream>
// using std::cout, std::endl;

// Backend
#include "../backend.h"

// servant
#include "servant/constants.h"
using namespace Front_consts;

namespace Frontend {

// из-за количества кнопок на экране конструктор сильно перегружен, но что поделать :)

Graphix_window::Graphix_window(Graph_lib::Point left_corner, int width, int height,
                               const string& title, int scale)
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
      point_box{width, height, cb_show_points, cb_hide_points}
{
    // не даём пользователю менять размеры окна
    size_range(width, height, width, height);

    // задаём цвет окну
    // this->color(Color::white);

    // инициализируем вектор введенных строк (пустыми)
    for (size_t i = 0; i < max_functions_amount; i++)
    {
        inputed_strings.push_back(empty_str);
        inputed_funcs.push_back(empty_str);
    }

    // задаём цвет осям
    x_axis->set_color(Color::Color_type::dark_cyan);
    y_axis->set_color(Color::Color_type::dark_cyan);

    // поскольку enter_menu не была инициализирована, а значит, мы создаем нулевой по
    // счету Function_box
    Function_box* func_box =
        new Function_box{0, cb_draw_graph, cb_hide_graph, cb_rem_func, cb_draw_der, cb_hide_der};
    func_box->set_index(0);
    // и добавляем его в общий список всех input_box'ов
    enter_menu.push_back(func_box);

    // аттачим всё
    attach(border);
    attach(*x_axis);
    attach(*y_axis);
    attach(quit_button);
    attach(incr_button);
    attach(decr_button);
    attach(new_button);

    func_box->attach(*this);
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
    dynamic_cast<Graphix_window&>(btn.window()).new_func();
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

        x_axis = new Axis{Axis::Orientation::horisontal, origin, win_w() - func_box_w, scale, "X"};
        x_axis->set_color(Color::Color_type::dark_cyan);

        y_axis = new Axis{Axis::Orientation::vertical, origin, win_h(), scale, "Y"};
        y_axis->set_color(Color::Color_type::dark_cyan);

        // аттачим новые оси
        attach(*x_axis);
        attach(*y_axis);

        // перересовываем все графики
        for (size_t i = 0; i < enter_menu.size(); ++i)
            if (!enter_menu[i]->is_graph_hidden())
                update_graph(i);

        // перересовываем все производные
        for (size_t i = 0; i < enter_menu.size(); ++i)
            if (!enter_menu[i]->is_der_hidden())
                update_der(i);

        // перерисовываем все точки, если они видны
        if (is_points_visible)
            update_points();
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

void Graphix_window::update_graph(size_t func_index)
{
    // чистим старый график и точки
    clear_graph(func_index);

    // записываем в вектор введенных строк то, что ввёл пользователь и заодно проверяем
    update_inputed_func(func_index);

    if (enter_menu[func_index]->is_input_valid())
    {
        // локальная переменная - введенная строка
        string func = inputed_funcs[func_index];

        // раскрываем все зависимости вида y_n
        auto edfc = Backend::expose_dep_func_string{inputed_funcs, func};

        // создаём сегментированную функцию
        Segmented_function seged_func(func, scale, origin, win_w(), win_h());
        // график - композиция нескольких графиков, каждый из которых определен на своём сегменте
        Vector_ref<Graphix> graphic = seged_func.get_segmented_function();

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

        enter_menu[func_index]->graph_show();
    }
}

void Graphix_window::draw_graph(size_t func_index)
{
    update_graph(func_index);
    button_pushed = true;
}

void Graphix_window::update_der(size_t der_index)
{
    // чистим старый график и точки
    clear_der(der_index);

    // записываем в вектор введенных строк то, что ввёл пользователь
    update_inputed_func(der_index);

    if (enter_menu[der_index]->is_input_valid())
    {
        // локальная переменная - введенная строка
        string func = inputed_funcs[der_index];

        // раскрываем все зависимости вида y_n
        auto edfc = Backend::expose_dep_func_string{inputed_strings, func};

        // создаём сегментированную функцию
        Segmented_function seged_func(func, scale, origin, win_w(), win_h());
        Vector_ref<Graphix> derivative = seged_func.get_segmented_derivative();

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

        // выводим производную
        enter_menu[der_index]->der_show();
    }
}

void Graphix_window::draw_der(size_t der_index)
{
    update_der(der_index);

    if (!enter_menu[der_index]->is_der_hidden())
        enter_menu[der_index]->set_der("(" + inputed_funcs[der_index] + ")'");

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
        enter_menu[j]->move(0, -func_box_h);
        enter_menu[j]->set_index(enter_menu[j]->get_index() - 1);
    }
    // также изменяем размеры самого вектора
    enter_menu.erase(enter_menu.begin() + func_index);

    if (graphics.size() > func_index)
    {
        // детачим этот график, его производную и чистим память
        clear_graph(func_index);
        clear_der(func_index);
        // также изменяем размеры самого вектора
        graphics.erase(graphics.begin() + func_index);
    }

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

void Graphix_window::new_func()
{
    // создаем новый бокс
    Function_box* func_box = new Function_box{enter_menu.size(), cb_draw_graph, cb_hide_graph,
                                              cb_rem_func,       cb_draw_der,   cb_hide_der};

    // задаём ему последний номер и аттачим его
    func_box->set_index(enter_menu.size());
    func_box->attach(*this);

    // если до увеличения массив инпут_боксов был пуст, то мы обратно двигаем new_button
    if (enter_menu.empty())
        new_button.move(func_box_w, 0);

    // добавляем новый инпут_бокс в соотв. вектор
    enter_menu.push_back(func_box);

    // увеличиваем размер вектора с графиками (резервируем под новую сегментированную функцию)
    graphics.push_back(Vector_ref<Graphix>{});

    // если количество графиков стало максимально, скрываем кнопку "new_button"
    if (enter_menu.size() == max_functions_amount)
        detach(new_button);

    button_pushed = true;
}

void Graphix_window::update_points()
{
    // чистим память от всех предыдущих точек
    clear_points();

    fill_inputed_funcs();

    // воспомогательная функция, которая переводит бэкендовы вещественные точки в пиксельные
    auto convert_to_pix = [&] (Math_calc::Point p) -> Graph_lib::Point
    {
        return Graph_lib::Point{origin.x + static_cast<int>(p.x * scale),
                                origin.y - static_cast<int>(p.y * scale)};
    };

    // переводим границы экрана в вещественные, чтобы использовать для бэкендовских функций

    double l_border = -((double)win_w() / (2 * scale)) + func_box_w / (2 * scale);
    double r_border = -l_border;
    double h_border = (double)win_h() / (2 * scale);
    double point_prec = (((double)win_w() / (scale * 2500)));

    // проходимся по всем строкам, куда пользователь вводит функции и рисуем их экстремумы, корни и
    // всевозможные пересечения (если они не скрыты)
    for (size_t i = 0; i < enter_menu.size(); i++)
    {
        auto& input_box = enter_menu[i];
        // введенная строка
        string& func = inputed_funcs[i];

        bool is_valid = input_box->is_input_valid();

        Marks* dots = new Marks{empty_str};

        if (is_valid)
        {
            // раскрываем все зависимости вида y_n

            auto edfc = Backend::expose_dep_func_string{inputed_strings, func};
            // cout << "func: " << func << endl;

            Math_calc::function_extremes fe{func, l_border, r_border, h_border, point_prec};
            // создаём марки, добавляем их на окно
            Marks* dots = new Marks{"#"};
            for (auto& p : fe.get_function_extremes())
                dots->add(convert_to_pix(p));
            attach(*dots);
            // и добавляем в общий массив всех точек на экране
            all_points.push_back(*dots);

            Math_calc::function_roots fr{func, l_border, r_border, h_border, point_prec};
            // создаём марки, добавляем их на окно
            dots = new Marks{"x"};
            for (auto& p : fr.get_function_roots())
                dots->add(convert_to_pix(p));
            attach(*dots);
            // и добавляем в общий массив всех точек на экране
            all_points.push_back(*dots);
        }

        for (size_t j = i + 1; j < enter_menu.size(); j++)
        {
            // "другой input_box"
            auto oth_input_box = enter_menu[j];
            string& oth_func = inputed_funcs[j];

            is_valid |= oth_input_box->is_input_valid();

            if (is_valid)
            {
                // раскрываем все зависимости вида y_n
                auto oth_edfc = Backend::expose_dep_func_string{inputed_strings, oth_func};

                Math_calc::function_crosses fc{
                    {func, oth_func}, l_border, r_border, h_border, point_prec};
                // создаём марки, добавляем их на окно
                dots = new Marks{"o"};
                for (auto& p : fc.get_functions_crosses())
                    dots->add(convert_to_pix(p));
                attach(*dots);
                // и добавляем в общий массив всех точек на экране
                all_points.push_back(*dots);
            }
        }
    }
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

void Graphix_window::clear_points()
{
    for (int i = 0; i < all_points.size(); i++)
        detach(all_points[i]);

    all_points.clear();
}

void Graphix_window::update_inputed_func(size_t func_index)
{
    // локальная переменная - введенная строка
    string& func = inputed_funcs[func_index];

    // cout << "inputed strings now:" << endl;
    for (size_t i = 0; i < enter_menu.size(); i++)
    {
        inputed_strings[i] = enter_menu[i]->get_string();
        // cout << i << ": " + inputed_strings[i] << endl;
    }
    // cout << endl << endl;

    func = inputed_strings[func_index];

    try
    {
        // раскрываем все зависимости вида y_n
        auto edfc = Backend::expose_dep_func_string{inputed_strings, func};

        // создаём сегментированную функцию (чтобы потенциально проверить на ошибки)
        Segmented_function seged_func(func, scale, origin, win_w(), win_h());

        // если попытка раскрытия зависимостей создания функции не вызвали ошибок, то всё
        // шикарно
        enter_menu[func_index]->set_message(empty_str);
        enter_menu[func_index]->input_valid();
    }
    catch (const std::exception& e)
    {
        // cout << "!update_inputed_func catched the error!" << endl;
        // выводим ошибку и прячем график
        enter_menu[func_index]->set_message(e.what());
        enter_menu[func_index]->input_invalid();
    }
    // cout << func_index << ": " + func + " " << enter_menu[func_index]->is_input_valid() << endl;
}

void Graphix_window::fill_inputed_funcs()
{
    for (size_t i = 0; i < enter_menu.size(); i++)
        update_inputed_func(i);
}

}  // namespace Frontend