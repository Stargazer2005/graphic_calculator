
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

namespace Frontend {

void Graphix_window::update_scale(double new_scale)
{
    // если выбранный масштаб допустим
    if (new_scale < max_scale && new_scale > min_scale)
    {
        unit_intr = new_scale;

        // детачим оси и высвобождаем память
        detach(*x_axis);
        detach(*y_axis);
        delete x_axis;
        delete y_axis;

        // создаём новые оси и задаём цвет

        x_axis = new Axis{Axis::Orientation::horisontal, origin, w() - func_box_w, unit_intr, "X"};
        x_axis->set_color(Color::Color_type::dark_cyan);

        y_axis = new Axis{Axis::Orientation::vertical, origin, h(), unit_intr, "Y"};
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
    // чистим старый график и точки
    clear_graphix(func_index);

    // записываем в вектор введенных строк то, что ввёл пользователь и заодно проверяем
    update_inputed_func(func_index);

    // если проверка прошла успешна, рисуем график по введённой строке
    if (enter_menu[func_index]->is_input_valid())
    {
        // локальная переменная - введенная строка
        auto func = inputed_funcs[func_index];

        // создаём сегментированную функцию
        Segmented_Graphix seged_func(func, unit_intr, origin, w(), h());

        // график - композиция нескольких графиков, каждый из которых определен на своём сегменте
        auto graphic = seged_func.get_segmented_graphix();

        //  аттачим сегментированную функцию
        for (const auto& graphic_part : graphic)
        {
            graphic_part->set_color(Color::black);
            attach(*graphic_part);
        }

        // записываем новую функцию в общий массив всех графиков
        if (graphics.size() > func_index)
            graphics[func_index] = graphic;
        else
            graphics.push_back(graphic);
    }
}

void Graphix_window::clear_graphix(size_t func_index)
{
    if (graphics.size() > func_index)
    {
        auto graphic = graphics[func_index];

        // убираем все спрятанные фрагменты графика с экрана
        for (const auto& graphic_part : graphic)
            detach(*graphic_part);

        graphics[func_index].clear();
    }

    clear_inputed_func(func_index);
}

void Graphix_window::update_deriv(size_t der_index)
{
    // чистим старый график и точки
    clear_deriv(der_index);

    // записываем в вектор введенных строк то, что ввёл пользователь
    update_inputed_func(der_index);

    // если проверка прошла успешна, рисуем график производной по введённой строке
    if (enter_menu[der_index]->is_input_valid())
    {
        // локальная переменная - введенная строка
        auto func = inputed_funcs[der_index];

        // создаём сегментированную функцию
        Segmented_Graphix seged_func(func, unit_intr, origin, w(), h());
        auto deriv = seged_func.get_segmented_deriv();

        // аттачим сегментированную функцию
        for (const auto& deriv_part : deriv)
        {
            deriv_part->set_color(Color::dark_red);
            attach(*deriv_part);
        }

        // записываем новую функцию в общий массив всех графиков
        if (derivs.size() > der_index)
            derivs[der_index] = deriv;
        else
            derivs.push_back(deriv);
    }
}

void Graphix_window::clear_deriv(size_t der_index)
{
    if (derivs.size() > der_index)
    {
        auto deriv = derivs[der_index];

        // убираем все спрятанные фрагменты графика с экрана
        for (const auto& deriv_part : deriv)
            detach(*deriv_part);

        derivs[der_index].clear();
    }
}

void Graphix_window::update_points()
{
    // чистим память от всех предыдущих точек
    clear_points();

    cout << "check1" << endl;

    fill_inputed_funcs();

    cout << "check2" << endl;

    // воспомогательная функция, которая переводит бэкендовы вещественные точки в пиксельные
    auto convert_to_pix = [&] (Math_calc::Point p) -> Graph_lib::Point {
        return {origin.x + int(p.x * unit_intr), origin.y - int(p.y * unit_intr)};
    };

    // переводим границы экрана в вещественные, чтобы использовать для бэкендовских функций

    double min_x = -((double)w() / (2 * unit_intr)) + func_box_w / (2 * unit_intr);
    double max_x = -min_x;
    double max_y = (double)h() / (2 * unit_intr);
    double point_prec = (((double)w() / (unit_intr * 2500)));

    // проходимся по всем строкам, куда пользователь вводит функции и рисуем их экстремумы, корни и
    // всевозможные пересечения (если они не скрыты)
    for (size_t i = 0; i < enter_menu.size(); i++)
    {
        auto function_box = enter_menu[i];

        auto func = inputed_funcs[i];

        bool is_valid = function_box->is_input_valid();

        Marks* dots = new Marks{empty_str};

        if (is_valid)
        {
            cout << "check3" << endl;
            Math_calc::function_roots fr{func, min_x, max_x, max_y, point_prec};
            cout << "check4" << endl;
            // создаём марки, добавляем их на окно
            dots = new Marks{"x"};
            for (const auto& p : fr.get_function_roots())
                dots->add(convert_to_pix(p));
            attach(*dots);

            // и добавляем в общий массив всех точек на экране
            all_points.push_back(dots);

            Math_calc::function_extremes fe{func, min_x, max_x, max_y, point_prec};
            cout << "check5" << endl;

            // создаём марки, добавляем их на окно
            Marks* dots = new Marks{"#"};
            for (const auto& p : fe.get_function_extremes())
                dots->add(convert_to_pix(p));
            attach(*dots);

            // и добавляем в общий массив всех точек на экране
            all_points.push_back(dots);
        }

        for (size_t j = i + 1; j < enter_menu.size(); j++)
        {
            // "другой function_box"
            auto oth_function_box = enter_menu[j];
            auto oth_func = inputed_funcs[j];

            if (oth_function_box->is_input_valid())
            {
                cout << "check6" << endl;
                Math_calc::function_crosses fc{{func, oth_func}, min_x, max_x, max_y, point_prec};
                cout << "check7" << endl;
                // создаём марки, добавляем их на окно
                dots = new Marks{"o"};
                for (const auto& p : fc.get_functions_crosses())
                    dots->add(convert_to_pix(p));
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
        detach(*points);

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

        // если попытка раскрытия зависимостей создания функции не вызвали ошибок, то всё
        // шикарно, предполагаемая функция действительно норм
        inputed_funcs[func_index] = func;

        enter_menu[func_index]->input_valid();
        enter_menu[func_index]->set_message(empty_str);
    }

    catch (const std::exception& e)
    {
        if (func_index < enter_menu.size())
        {
            // inputed_funcs[func_index] = empty_func;
            // выводим ошибку и прячем график
            enter_menu[func_index]->set_message(string{e.what()});
            enter_menu[func_index]->input_invalid();
        }

        if (string{e.what()} == "self-usage or loop")
        {
            // print(dependences);
            // апдейтим все зависимые функции
            for (const auto& n : dependences)
                update_inputed_func(n - 1, false);
        }
    }
}

void Graphix_window::clear_inputed_func(size_t func_index)
{
    inputed_funcs[func_index] = empty_func;
}

void Graphix_window::fill_inputed_funcs()
{
    for (size_t i = 0; i < enter_menu.size(); i++)
        update_inputed_func(i);
}

}  // namespace Frontend