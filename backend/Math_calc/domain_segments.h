#pragma once

// std libs
#include <functional>
#include <vector>

// Math_calc
#include "math_base.h"

namespace Math_calc {

// MEMO: функция, которая дробит на вещественные отрезки
std::vector<Segment> domain_segments (const std::function<double(double)>& calc, double min_x,
                                      double max_x, double max_y, double precision);

}  // namespace Math_calc