#pragma once

// std libs
#include <functional>
using std::function;

// Graph_lib
#include <Graph_lib/Graph.h>
using Graph_lib::Function;
using Graph_lib::Point;
using Graph_lib::Shape;

namespace Graphix_calc {

// фигура, представляющая собой график f(x) с областью определения [l_border:r_border]
class Graphix : public Shape
{
  public:
    Graphix(function<double(double)> func, double l_border, double r_border,
            Graph_lib::Point center, int point_amount, int scale);
};

}  // namespace Graphix_calc
