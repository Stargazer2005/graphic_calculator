#pragma once
#include "num_button.h"
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

using Graph_lib::In_box;
using Graph_lib::Point;
using Graphix_calc::Numbered_button;

namespace Graphix_calc {
class Input_box
{
  public:
    Input_box() {}

    // а дальше идёт то, что нарушает инкапсуляцию...
    In_box* in_box;
    Numbered_button* draw_button;
    Numbered_button* hide_button;
    Numbered_button* rem_button;
    bool is_hidden;
};

}  // namespace Graphix_calc