#pragma once
#include "num_button.h"
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

using Graph_lib::In_box;
using Graphix_calc::Button_num;

namespace Graphix_calc {
struct Input_box
{
    Input_box() {}

    In_box* in_box;
    Button_num* draw_button;
    Button_num* hide_button;
    bool is_hidden;
};
}  // namespace Graphix_calc