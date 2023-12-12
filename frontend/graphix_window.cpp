// header
#include "graphix_window.h"

// Graphix_calc
#include "Graphix_calc/segmented_function.h"

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
      new_graph_button{Point{inp_box_w + scl_btn_side, 0}, btn_w, btn_h, "New graph", cb_new}
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

        button_pushed = true;
    }
}

void Graphix_window::increase_scale() { change_scale(scale * scale_coef); }

void Graphix_window::decrease_scale() { change_scale(scale / scale_coef); }

void Graphix_window::draw_graph(size_t graph_number)
{
    // прячем старый график
    hide_graph(graph_number);

    // высвобождаем память
    if (graphics.size() > graph_number)
        graphics[graph_number].clear();

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
    // убираем все спрятанные фрагменты графика с экрана
    if (graphics.size() > graph_number)
        for (int j = 0; j < graphics[graph_number].size(); ++j)
            detach(graphics[graph_number][j]);

    // прячем окно
    enter_menu[graph_number]->hide();

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
        // теперь проходимся по всем фрагментам графика и детачим их
        for (int j = 0; j < graphics[graph_number].size(); ++j)
            detach(graphics[graph_number][j]);
        // также изменяем размеры самого вектора
        graphics.erase(graphics.begin() + graph_number);
    }

    button_pushed = true;
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

}  // namespace Frontend