#pragma once
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

using Graph_lib::Button;

namespace Graphix_calc {
struct Numbered_button : Button
{
    using Button::Button;
    int number;
};
}  // namespace Graphix_calc