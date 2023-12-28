#pragma once

// std libs
#include <functional>
#include <vector>

// Graph_lib
#include "../Graph_lib/Graph.h"

namespace Graphix_calc {

// фигура, представляющая собой график f(x) с областью определения [min_x:max_x]
class Graphix : public Graph_lib::Shape
{
  public:
    Graphix(std::function<double(double)> func, double min_x, double max_x, Graph_lib::Point origin,
            int point_amount, double unit_intr);
};

}  // namespace Graphix_calc
