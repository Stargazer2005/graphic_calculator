// header
#include "Input_box.h"

// servant
#include "../servant/constants.h"

using namespace Front_consts;

using Graph_lib::Point;

namespace Graphix_calc {

Input_box::Input_box(short graphics_amount, void cb_draw(void*, void* widget),
                     void cb_hide(void*, void* widget), void cb_rem(void*, void* widget))
    : Graph_lib::Widget{Point{inp_box_h, inp_box_indent * graphics_amount}, inp_box_w, inp_box_h,
                        "y = ", nullptr},
      in_box{new In_box{Point{inp_box_h, inp_box_indent * graphics_amount}, inp_box_w, inp_box_h,
                        "y = "}},
      draw_button{new Numbered_button{Point{0, inp_box_indent * graphics_amount + inp_box_h}, btn_w,
                                      btn_h, "Draw", cb_draw}},
      hide_button{
          new Numbered_button{Point{btn_w + padding, inp_box_indent * graphics_amount + inp_box_h},
                              btn_w, btn_h, "Hide", cb_hide}},
      rem_button{new Numbered_button{
          Point{(btn_w + padding) * 2, inp_box_indent * graphics_amount + inp_box_h}, btn_w, btn_h,
          "Remove", cb_rem}}
{
}

bool Input_box::is_hidden() const { return !is_graph_visible; }

void Input_box::show() { is_graph_visible = true; }

void Input_box::hide() { is_graph_visible = false; }

void Input_box::move(int dx, int dy)
{
    in_box->move(dx, dy);
    draw_button->move(dx, dy);
    hide_button->move(dx, dy);
    rem_button->move(dx, dy);
}

std::string Input_box::get_string() const { return in_box->get_string(); }

int Input_box::get_number() const { return draw_button->get_number(); }

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
    own = &win;
}

void Input_box::detach(Graph_lib::Window& win)
{
    win.detach(*in_box);
    win.detach(*draw_button);
    win.detach(*hide_button);
    win.detach(*rem_button);
    own = &win;
}

}  // namespace Graphix_calc