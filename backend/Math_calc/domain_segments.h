#pragma once

// std libs
#include <functional>
#include <vector>

// Math_calc
#include "math_base.h"

namespace Math_calc {

// RETURNS: вещественные отрезки, где мат. функция определена
// (по лямбде от мат. функции, мин, макс значениям (краям) и точности)
std::vector<Segment> domain_segments (const std::function<double(double)>& calc, double min_x,
                                      double max_x, double max_y, double precision);

}  // namespace Math_calc