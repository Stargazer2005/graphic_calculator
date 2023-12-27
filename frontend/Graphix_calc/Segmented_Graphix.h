#pragma once

// std libs
#include <string>

// Graph_lib
#include "../Graph_lib/Graph.h"

// Graphix_calc
#include "Graphix.h"

// Backend
#include "../../backend.h"

namespace Graphix_calc {

// функция, поделенная на сегменты, связанные с её областью определения
class Segmented_Graphix
{
  public:
    Segmented_Graphix(Math_func::function _func, double unit_intr, Graph_lib::Point origin,
                      int r_border, int h_border);

    // methods

    std::vector<Graphix_calc::Graphix*> get_segmented_graphix () const { return seged_graphix; }

    std::vector<Graphix_calc::Graphix*> get_segmented_deriv () const { return seged_deriv; }

  private:
    // vars

    // значение строки
    Math_func::function func;
    // список отрезков, на котором рисуем функцию
    std::vector<Math_calc::Segment> func_segs;
    std::vector<Math_calc::Segment> segs_der;

    // отсегментированная функция
    std::vector<Graphix_calc::Graphix*> seged_graphix;
    std::vector<Graphix_calc::Graphix*> seged_deriv;

    // methods

    std::vector<Math_calc::Segment> derivative_segment (int l_border, int r_border, int h_border,
                                                        double unit_intr) const;
    std::vector<Math_calc::Segment> segments (int l_border, int r_border, int h_border,
                                              double unit_intr) const;

    // функция, создающая вектор Graphix - раздробленная функция на отрезки
    std::vector<Graphix_calc::Graphix*> segmented_graphix (double unit_intr, Graph_lib::Point center,
                                                           int max_x) const;
    std::vector<Graphix_calc::Graphix*> segmented_deriv (double unit_intr, Graph_lib::Point center,
                                                         int max_x) const;
};

}  // namespace Graphix_calc