#pragma once

// Graph_lib
#include "../Graph_lib/GUI.h"
#include "../Graph_lib/Window.h"

namespace Graphix_calc {

class Point_box : public Graph_lib::Widget
{
  public:
    // упрощенный конструктор
    Point_box(int window_w, int window_h, void cb_show(void*, void*), void cb_hide(void*, void*));

    // methods

    // attach all buttons
    void attach (Graph_lib::Window&);

    // detach all buttons
    void detach (Graph_lib::Window&);

  private:
    // vars

    Graph_lib::Button* show_button;
    Graph_lib::Button* hide_button;
};

}  // namespace Graphix_calc