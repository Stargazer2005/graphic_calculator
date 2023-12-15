// header
#include "Input_box.h"

// servant
#include "../servant/constants.h"
using namespace Front_consts;

namespace Graphix_calc {

// да, конструктор перегружен, но лучше пусть это будет здесь, чем по несколько раз в Graphix_window
Input_box::Input_box(short graphics_amount, void cb_draw(void*, void*), void cb_hide(void*, void*),
                     void cb_rem(void*, void*))
    : Graph_lib::Widget{Point{0, 0}, 1, 1, "", nullptr},
      in_box{new In_box{Point{inp_box_h, inp_box_indent_h * graphics_amount}, inp_box_w, inp_box_h,
                        "y = "}},
      draw_button{new Numbed_button{Point{0, inp_box_indent_h * graphics_amount + inp_box_h}, btn_w,
                                    btn_h, "Draw", cb_draw}},
      hide_button{
          new Numbed_button{Point{btn_w + padding, inp_box_indent_h * graphics_amount + inp_box_h},
                            btn_w, btn_h, "Hide", cb_hide}},
      rem_button{new Numbed_button{
          Point{(btn_w + padding) * 2, inp_box_indent_h * graphics_amount + inp_box_h}, btn_w,
          btn_h, "Remove", cb_rem}},
      out_box{new Out_box{Point{0, inp_box_indent_h * graphics_amount + inp_box_h + btn_h},
                          out_box_w, out_box_h, ""}}
{
}

bool Input_box::is_hidden() const { return !is_graph_visible; }

void Input_box::graph_show() { is_graph_visible = true; }

void Input_box::graph_hide() { is_graph_visible = false; }

void Input_box::move(int dx, int dy)
{
    in_box->move(dx, dy);
    draw_button->move(dx, dy);
    hide_button->move(dx, dy);
    rem_button->move(dx, dy);
    out_box->move(dx, dy);
}

void Input_box::set_number(int value)
{
    draw_button->set_number(value);
    hide_button->set_number(value);
    rem_button->set_number(value);
}

void Input_box::attach(Graph_lib::Window& win)
{
    win.attach(*in_box);
    win.attach(*draw_button);
    win.attach(*hide_button);
    win.attach(*rem_button);
    win.attach(*out_box);
    own = &win;
}

void Input_box::set_message(const string& str) { out_box->put(str); }

void Input_box::detach(Graph_lib::Window& win)
{
    win.detach(*in_box);
    win.detach(*draw_button);
    win.detach(*hide_button);
    win.detach(*rem_button);
    win.detach(*out_box);
    own = &win;
}

}  // namespace Graphix_calc