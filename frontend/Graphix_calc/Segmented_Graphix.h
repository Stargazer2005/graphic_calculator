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
    Segmented_Graphix(Backend::function _func, double scale, Graph_lib::Point origin, int max_x,
                      int max_y);

    // methods

    std::vector<Graphix_calc::Graphix*> get_segmented_graphix () const { return seged_graphix; }

    std::vector<Graphix_calc::Graphix*> get_segmented_deriv () const { return seged_deriv; }

  private:
    // variables

    // значение строки
    Backend::function func;
    // список отрезков, на котором рисуем функцию
    std::vector<Math_calc::Segment> func_segs;
    std::vector<Math_calc::Segment> segs_der;

    // отсегментированная функция
    std::vector<Graphix_calc::Graphix*> seged_graphix;
    std::vector<Graphix_calc::Graphix*> seged_deriv;

    // methods

    std::vector<Math_calc::Segment> derivative_segment (int max_x, int max_y, double scale) const;
    std::vector<Math_calc::Segment> segments (int max_x, int max_y, double scale) const;

    // функция, создающая вектор Graphix - раздробленная функция на отрезки
    std::vector<Graphix_calc::Graphix*> segmented_graphix (double scale, Graph_lib::Point center,
                                                           int max_x) const;
    std::vector<Graphix_calc::Graphix*> segmented_deriv (double scale, Graph_lib::Point center,
                                                         int max_x) const;
};

}  // namespace Graphix_calc