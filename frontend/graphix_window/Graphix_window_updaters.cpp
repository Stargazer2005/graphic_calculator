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

// utility
#include "../utility/constants.h"
using namespace Frontend_consts;
#include "../utility/utilities.h"
using namespace Frontend_utilities;

namespace Graphix_win {

void Graphix_window::update_unit_intr(double new_unit_intr)
{
    // если выбранный масштаб допустим
    if (new_unit_intr < max_unit_intr && new_unit_intr > min_unit_intr)
    {
        unit_intr = new_unit_intr;

        // детачим оси и высвобождаем память
        detach(*x_axis);
        detach(*y_axis);
        delete x_axis;
        delete y_axis;

        // создаём новые оси и задаём цвет

        x_axis = new Axis{
            Axis::Orientation::horisontal, origin, w() - func_box_w, unit_intr, mark_intr, "X"};
        x_axis->set_color(Color::Color_type::dark_cyan);

        y_axis = new Axis{Axis::Orientation::vertical, origin, h(), unit_intr, mark_intr, "Y"};
        y_axis->set_color(Color::Color_type::dark_cyan);

        // аттачим новые оси
        attach(*x_axis);
        attach(*y_axis);

        // перересовываем все графики
        for (size_t i = 0; i < enter_menu.size(); ++i)
            if (!enter_menu[i]->is_graph_hidden())
                update_graphix(i);

        // перересовываем все производные
        for (size_t i = 0; i < enter_menu.size(); ++i)
            if (!enter_menu[i]->is_der_hidden())
                update_deriv(i);

        // перерисовываем все точки, если они видны
        if (is_points_visible)
            update_points();
    }
}

void Graphix_window::update_graphix(size_t func_index)
{
    // записываем в вектор введенных строк то, что ввёл пользователь и заодно проверяем
    update_inputed_func(func_index);

    // если проверка прошла успешна, рисуем график по введённой строке
    if (enter_menu[func_index]->is_input_valid())
    {
        // локальная переменная - введенная строка
        auto func = inputed_funcs[func_index];

        // создаём сегментированную функцию
        auto seged_graphix =
            new Segmented_Graphix(func.calculate, unit_intr, origin, {func_box_w, h()}, {w(), 0});

        seged_graphix->set_color(Color::black);

        attach(*seged_graphix);

        // записываем новую функцию в общий массив всех графиков
        clear_graphix(func_index);
        graphics[func_index] = seged_graphix;
    }
}

void Graphix_window::clear_graphix(size_t func_index, bool need_delete)
{
    auto& seged_graphix = graphics[func_index];

    detach(*seged_graphix);

    if (need_delete)
        delete seged_graphix;
}

void Graphix_window::update_deriv(size_t der_index)
{
    // записываем в вектор введенных строк то, что ввёл пользователь
    update_inputed_func(der_index);

    // если проверка прошла успешна, рисуем график производной по введённой строке
    if (enter_menu[der_index]->is_input_valid())
    {
        // локальная переменная - введенная строка
        auto func = inputed_funcs[der_index];

        // создаём сегментированную функцию
        auto seged_deriv = new Segmented_Graphix(func.differentiate, unit_intr, origin,
                                                 {func_box_w, h()}, {w(), 0});

        seged_deriv->set_color(Color::dark_red);

        attach(*seged_deriv);

        // записываем новую функцию в общий массив всех графиков
        clear_deriv(der_index);
        derivs[der_index] = seged_deriv;
    }
}

void Graphix_window::clear_deriv(size_t der_index, bool need_delete)
{
    auto& seged_deriv = derivs[der_index];

    detach(*seged_deriv);

    if (need_delete)
        delete seged_deriv;
}

void Graphix_window::update_points()
{
    // чистим память от всех предыдущих точек
    clear_points();

    fill_inputed_funcs();

    // переводим границы экрана в вещественные, чтобы использовать для бэкендовских функций
    auto left_bottom = convert_to_real(origin, {func_box_w, h()}, unit_intr);
    auto right_top = convert_to_real(origin, {w(), 0}, unit_intr);

    // FIXME: естественно, она не должна быть константой
    double point_prec = 0.0001;

    // проходимся по всем строкам, куда пользователь вводит функции и рисуем их экстремумы, корни и
    // всевозможные пересечения (если они не скрыты)
    for (size_t i = 0; i < enter_menu.size(); i++)
    {
        auto function_box = enter_menu[i];

        auto func = inputed_funcs[i];

        bool is_valid = function_box->is_input_valid();

        Marks* dots = new Marks{empty_str};

        vector<Math_calc::Point> points;

        if (is_valid)
        {

            points = vector<Math_calc::Point>(
                Math_calc::function_roots{func, left_bottom, right_top, point_prec});
            // создаём марки, добавляем их на окно
            dots = new Marks{"x"};
            for (const auto& p : points)
                dots->add(convert_to_pix(origin, p, unit_intr));
            attach(*dots);

            // и добавляем в общий массив всех точек на экране
            all_points.push_back(dots);

            points = vector<Math_calc::Point>(
                Math_calc::function_extremes{func, left_bottom, right_top, point_prec});

            // создаём марки, добавляем их на окно
            Marks* dots = new Marks{"#"};
            for (const auto& p : points)
                dots->add(convert_to_pix(origin, p, unit_intr));
            attach(*dots);

            // и добавляем в общий массив всех точек на экране
            all_points.push_back(dots);
        }

        for (size_t j = i + 1; j < enter_menu.size(); j++)
        {
            // "другой function_box"
            auto oth_function_box = enter_menu[j];
            auto oth_func = inputed_funcs[j];

            if (is_valid && oth_function_box->is_input_valid())
            {
                points = vector<Math_calc::Point>(Math_calc::function_crosses{
                    {func, oth_func}, left_bottom, right_top, point_prec});

                // создаём марки, добавляем их на окно
                dots = new Marks{"o"};
                for (const auto& p : points)
                    dots->add(convert_to_pix(origin, p, unit_intr));
                attach(*dots);
                // и добавляем в общий массив всех точек на экране
                all_points.push_back(dots);
            }
        }
    }
}

void Graphix_window::clear_points()
{
    for (const auto& points : all_points)
    {
        detach(*points);
        delete points;
    }

    all_points.clear();
}

void Graphix_window::update_inputed_func(size_t func_index, bool need_update_strings)
{
    if (need_update_strings)
    {
        // считываем все строки
        for (size_t i = 0; i < enter_menu.size(); i++)
            inputed_strings[i] = enter_menu[i]->get_string();
    }

    // локальная переменная - введенная строка (возможная функция)
    string estimated_func_str = inputed_strings[func_index];

    // заготовка под зависимости для введенной функции
    vector<size_t> dependences;

    try
    {
        // раскрываем все зависимости вида y_n
        auto edfc = Math_func::expose_func_str{inputed_strings, estimated_func_str};
        dependences = edfc.get_dependences();

        // апдейтим все зависимые функции
        for (const auto& n : dependences)
            update_inputed_func(n - 1, false);

        // создаём функцию (чтобы потенциально проверить на ошибки)
        Math_func::function func{estimated_func_str};

        cout << func.get_func_str() << endl;

        // если попытка раскрытия зависимостей создания функции не вызвали ошибок, то всё
        // шикарно, предполагаемая функция действительно норм
        inputed_funcs[func_index] = func;

        cout << inputed_funcs[func_index].get_func_str() << endl;

        enter_menu[func_index]->input_valid();
        enter_menu[func_index]->set_message(empty_str);
    }

    catch (const std::exception& e)
    {
        if (func_index < enter_menu.size())
        {
            clear_graphix(func_index, false);
            enter_menu[func_index]->set_message(string{e.what()});
            enter_menu[func_index]->input_invalid();
        }
    }
}

void Graphix_window::fill_inputed_funcs()
{
    for (size_t i = 0; i < enter_menu.size(); i++)
        update_inputed_func(i);
}

}  // namespace Graphix_win