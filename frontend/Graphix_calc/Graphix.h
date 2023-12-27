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
    Graphix(std::function<double(double)> func, double min_x, double max_x,
            Graph_lib::Point center, int point_amount, double unit_intr);

    std::vector<Graph_lib::Point> get_points () const { return points; }

  private:
    void add_point(Graph_lib::Point);
    std::vector<Graph_lib::Point> points;
};

}  // namespace Graphix_calc
