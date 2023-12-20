#pragma once

// std libs
#include <string>

// Graph_lib
#include <Graph_lib/Graph.h>

// Graphix_calc
#include "Graphix.h"

// Backend
#include "../../backend.h"

namespace Graphix_calc {

// функция, поделенная на сегменты, связанные с её областью определения
class Segmented_function
{
  public:
    Segmented_function(const std::string& func, double scale, Graph_lib::Point origin, int max_x,
                       int max_y);

    // methods

    Graph_lib::Vector_ref<Graphix_calc::Graphix> get_segmented_function () const
    {
        return seg_func;
    }

    Graph_lib::Vector_ref<Graphix_calc::Graphix> get_segmented_derivative () const
    {
        return seg_der;
    }

  private:
    // variables

    // значение строки
    Backend::math_function func_str;
    bool has_var;
    // список отрезков, на котором рисуем функцию
    std::vector<Math_calc::Segment> segs_func;
    std::vector<Math_calc::Segment> segs_der;
    // отсегментированная функция
    Graph_lib::Vector_ref<Graphix_calc::Graphix> seg_func;
    Graph_lib::Vector_ref<Graphix_calc::Graphix> seg_der;

    // methods

    std::vector<Math_calc::Segment> derivative_segment (int max_x, int max_y, double scale) const;
    std::vector<Math_calc::Segment> segments (int max_x, int max_y, double scale) const;

    // функция, создающая вектор Graphix - раздробленная функция на отрезки
    Graph_lib::Vector_ref<Graphix_calc::Graphix>
    segmented_function (double scale, Graph_lib::Point center, int max_x) const;
    Graph_lib::Vector_ref<Graphix_calc::Graphix>
    segmented_derivative (double scale, Graph_lib::Point center, int max_x) const;
};

}  // namespace Graphix_calc