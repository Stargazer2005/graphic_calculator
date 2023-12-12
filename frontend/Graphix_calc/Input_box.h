#pragma once

// Graphix_calc
#include "Numbered_button.h"

// Graph_lib
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>
#include <Graph_lib/Window.h>

using Graph_lib::In_box;
using Graphix_calc::Numbered_button;

namespace Graphix_calc {

struct Input_box : Graph_lib::Widget
{
    Input_box(short graphics_amount, void cb_draw(void*, void* widget),
              void cb_hide(void*, void* widget), void cb_rem(void*, void* widget));

    bool is_hidden ();

    void show () override;

    void hide () override;

    void move (int dx, int dy) override;

    std::string get_string ();

    int get_number ();

    void set_number (int value);

    // attach all buttons
    void attach (Graph_lib::Window& win);

    // detach all buttons
    void detach (Graph_lib::Window& win);

  private:
    In_box* in_box;
    Numbered_button* draw_button;
    Numbered_button* hide_button;
    Numbered_button* rem_button;
    bool visibility;
};

}  // namespace Graphix_calc