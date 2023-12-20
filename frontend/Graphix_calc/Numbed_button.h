#pragma once

// Graph_lib
#include <Graph_lib/GUI.h>

namespace Graphix_calc {

// класс кнопки, которая имеет номер
class Numbed_button : public Graph_lib::Button
{
  public:
    using Graph_lib::Button::Button;

    // methods

    int get_number () const { return number; }

    void set_number (int value) { number = value; }

  private:
    // variables

    int number;
};

}  // namespace Graphix_calc