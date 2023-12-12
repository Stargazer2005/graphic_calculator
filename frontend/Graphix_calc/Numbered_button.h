#pragma once

// Graph_lib
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

using Graph_lib::Button;

namespace Graphix_calc {

// класс кнопки, которая имеет номер
struct Numbered_button : Button
{
    using Button::Button;

    int get_number () { return number; }

    void set_number (int value) { number = value; }

  private:
    int number;
};

}  // namespace Graphix_calc