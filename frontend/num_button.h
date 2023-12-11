#pragma once
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

using Graph_lib::Button;

namespace Graphix_calc {

class Numbered_button : public Button
{
  public:
    using Button::Button;

    void set_number (int value) { number = value; }

    int get_number () { return number; }

  private:
    int number;
};

}  // namespace Graphix_calc