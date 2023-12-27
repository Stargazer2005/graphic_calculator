#pragma once

// std libs
#include <string>

// Graphix_calc
#include "Numbed_button.h"

// Graph_lib
#include "../Graph_lib/GUI.h"
#include "../Graph_lib/Window.h"

namespace Graphix_calc {

// структурка, содержащая в себе ввод, кнопки по работе с графиком, производной
class Function_box : public Graph_lib::Widget
{
  public:
    // упрощенный конструктор
    // FIXME: это фигня, а не конструктор, всё, блин, держится на константах
    Function_box(unsigned long long int box_index, void cb_draw(void*, void*),
                 void cb_hide(void*, void*), void cb_rem(void*, void*),
                 void cb_draw_der(void*, void*), void cb_hide_der(void*, void*));

    // methods

    void input_valid () { is_func_valid = true; }

    void input_invalid () { is_func_valid = false; }

    bool is_input_valid () const { return is_func_valid; };

    void graph_show () { is_graph_visible = true; }

    void graph_hide () { is_graph_visible = false; }

    bool is_graph_hidden () const { return !is_graph_visible; }

    void der_show () { is_der_visible = true; }

    void der_hide () { is_der_visible = false; }

    bool is_der_hidden () const { return !is_der_visible; }

    void set_index (int);

    void set_message (const std::string& message);

    void set_der_str (const std::string& der_str) { curr_der->put(der_str); }

    std::string get_string () const { return in_box->get_string(); }

    int get_index () const { return draw_button->get_number(); }

    // attach all buttons
    void attach (Graph_lib::Window&) override;

    // detach all buttons
    void detach (Graph_lib::Window&);

    // just move all elements
    void move (int dx, int dy) override;

  private:
    // vars

    // поле, куда пользователь вводит функцию
    Graph_lib::In_box* in_box;
    Numbed_button* draw_button;
    Numbed_button* hide_button;
    Numbed_button* rem_button;
    Graph_lib::Out_box* curr_der;
    Numbed_button* draw_der_button;
    Numbed_button* hide_der_button;
    Graph_lib::Out_box* out_box;

    bool is_func_valid{false};
    bool is_graph_visible{false};
    bool is_der_visible{false};
};

}  // namespace Graphix_calc