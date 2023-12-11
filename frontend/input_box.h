#pragma once
#include "num_button.h"
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

using Graph_lib::In_box;
using Graphix_calc::Numbered_button;

namespace Graphix_calc {
struct Input_box
{
    Input_box() {}

    In_box* in_box;
    Numbered_button* draw_button;
    Numbered_button* hide_button;
    Numbered_button* rem_button;
    bool is_hidden;
};
}  // namespace Graphix_calc