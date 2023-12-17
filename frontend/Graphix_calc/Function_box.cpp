// header
#include "Function_box.h"

// servant
#include "../servant/constants.h"
using namespace Front_consts;

namespace Graphix_calc {

// да, конструктор перегружен, но лучше пусть это будет здесь, чем по несколько раз в Graphix_window
Function_box::Function_box(short graphics_amount, void cb_draw(void*, void*),
                           void cb_hide(void*, void*), void cb_rem(void*, void*),
                           void cb_draw_der(void*, void*), void cb_hide_der(void*, void*))
    : Graph_lib::Widget{Point{0, 0}, 1, 1, "", nullptr},
      in_box{new In_box{Point{in_box_h, function_box_h * graphics_amount}, in_box_w, in_box_h,
                        "y = "}},
      draw_button{new Numbed_button{Point{0, function_box_h * graphics_amount + in_box_h}, btn_w,
                                    btn_h, "Draw", cb_draw}},
      hide_button{
          new Numbed_button{Point{btn_w + padding, function_box_h * graphics_amount + in_box_h},
                            btn_w, btn_h, "Hide", cb_hide}},
      rem_button{new Numbed_button{
          Point{(btn_w + padding) * 2, function_box_h * graphics_amount + in_box_h}, btn_w, btn_h,
          "Remove", cb_rem}},
      draw_der_button{new Numbed_button{
          Point{(btn_w + padding) / 2, function_box_h * graphics_amount + in_box_h + btn_h}, btn_w,
          btn_h, "Draw_der", cb_draw_der}},
      hide_der_button{new Numbed_button{
          Point{3 * (btn_w + padding) / 2, function_box_h * graphics_amount + in_box_h + btn_h},
          btn_w, btn_h, "Hide_der", cb_hide_der}},
      out_box{new Out_box{Point{0, function_box_h * graphics_amount + in_box_h + 2 * btn_h},
                          out_box_w, out_box_h, ""}}
{
}

bool Function_box::is_graph_hidden() const { return !is_graph_visible; }

void Function_box::graph_show() { is_graph_visible = true; }

void Function_box::graph_hide() { is_graph_visible = false; }

bool Function_box::is_der_hidden() const { return !is_der_visible; }

void Function_box::der_show() { is_der_visible = true; }

void Function_box::der_hide() { is_der_visible = false; }

void Function_box::move(int dx, int dy)
{
    in_box->move(dx, dy);
    draw_button->move(dx, dy);
    hide_button->move(dx, dy);
    rem_button->move(dx, dy);
    draw_der_button->move(dx, dy);
    hide_der_button->move(dx, dy);
    out_box->move(dx, dy);
}

void Function_box::set_number(int value)
{
    draw_button->set_number(value);
    hide_button->set_number(value);
    rem_button->set_number(value);
    draw_der_button->set_number(value);
    hide_der_button->set_number(value);
}

void Function_box::attach(Graph_lib::Window& win)
{
    win.attach(*in_box);
    win.attach(*draw_button);
    win.attach(*hide_button);
    win.attach(*rem_button);
    win.attach(*draw_der_button);
    win.attach(*hide_der_button);
    win.attach(*out_box);
    own = &win;
}

void Function_box::set_message(const string& str) { out_box->put(str); }

void Function_box::detach(Graph_lib::Window& win)
{
    win.detach(*in_box);
    win.detach(*draw_button);
    win.detach(*hide_button);
    win.detach(*rem_button);
    win.detach(*draw_der_button);
    win.detach(*hide_der_button);
    win.detach(*out_box);
    own = &win;
}

}  // namespace Graphix_calc