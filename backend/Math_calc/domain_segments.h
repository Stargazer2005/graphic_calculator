#pragma once

// std libs
#include <functional>
#include <vector>

// Math_calc
#include "math_base.h"

namespace Math_calc {

// DOES: дробит на вещественные отрезки по лямбде, мин., макс. значениям и точности
std::vector<Segment> domain_segments (const std::function<double(double)>& calc, double min_x,
                                      double max_x, double max_y, double precision);

}  // namespace Math_calc