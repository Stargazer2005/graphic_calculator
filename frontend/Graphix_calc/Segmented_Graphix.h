#pragma once

// std libs
#include <functional>
#include <string>

// Graph_lib
#include "../Graph_lib/Graph.h"

// Graphix_calc
#include "Graphix.h"

// Backend
#include "../../backend.h"

namespace Graphix_calc {

// функция, поделенная на сегменты, связанные с её областью определения
class Segmented_Graphix : public Graph_lib::Shape
{
  public:
    Segmented_Graphix() = default;
    Segmented_Graphix(std::function<double(double)> _calc, double unit_intr,
                      Graph_lib::Point origin, Graph_lib::Point left_bottom,
                      Graph_lib::Point right_top);

    // methods

    void set_color (Graph_lib::Color c);

  private:
    // vars

    std::function<double(double)> calc;

    // список отрезков, на котором рисуем функцию
    std::vector<Math_calc::Segment> func_segs;

    // отсегментированная функция
    std::vector<Graphix_calc::Graphix*> seged_graphix;

    // methods

    void draw_lines () const override;

    std::vector<Math_calc::Segment> segments (double unit_intr, Graph_lib::Point origin,
                                              Graph_lib::Point left_bottom,
                                              Graph_lib::Point right_top) const;

    // функция, создающая вектор Graphix - раздробленная функция на отрезки
    std::vector<Graphix_calc::Graphix*> segmented_graphix (double unit_intr,
                                                           Graph_lib::Point origin,
                                                           Graph_lib::Point left_bottom,
                                                           Graph_lib::Point right_top) const;
};

}  // namespace Graphix_calc