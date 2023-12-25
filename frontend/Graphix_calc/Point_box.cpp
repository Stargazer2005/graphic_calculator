
#include "Point_box.h"

// Graph_lib
using Graph_lib::Button;
using Graph_lib::Point;

// utility
#include "../utility/constants.h"

using namespace Frontend_consts;

namespace Graphix_calc {

Point_box::Point_box(int window_w, int window_h, void cb_show(void*, void*),
                     void cb_hide(void*, void*))
    : Widget{Point{window_w, window_h}, 1, 1, "", nullptr},
      show_button{new Button{Point{window_w - btn_w * 2 - padding, window_h - btn_h}, btn_w, btn_h,
                             "Show", cb_show}},
      hide_button{
          new Button{Point{window_w - btn_w, window_h - btn_h}, btn_w, btn_h, "Hide", cb_hide}}
{
}

void Point_box::attach(Graph_lib::Window& win)
{
    win.attach(*show_button);
    win.attach(*hide_button);
    own = &win;
}

void Point_box::detach(Graph_lib::Window& win)
{
    win.attach(*show_button);
    win.attach(*hide_button);
    own = &win;
}

}  // namespace Graphix_calc
