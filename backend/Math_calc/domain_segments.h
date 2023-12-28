#pragma once

// std libs
#include <functional>
#include <vector>

// Math_calc
#include "math_base.h"

namespace Math_calc {

// RETURNS: вещественные отрезки, где мат. функция определена
// ARGS: лямбда-выражение от мат. функции, правая нижняя точка - начало деления на сегменты,
// ARGS: левая верхняя точка - конец деления на сегменты, точность
// (деление на сегменты происходит на прямоугольной области)
std::vector<Segment> domain_segments (const std::function<double(double)>& calc,
                                      Math_calc::Point left_bottom, Math_calc::Point right_top,
                                      double precision = 0.00001);

}  // namespace Math_calc