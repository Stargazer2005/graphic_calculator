#pragma once

// std libs
#include <functional>
#include <vector>

// Math_calc
#include "math_base.h"  // for Segment

namespace Math_calc {

// функция, которая дробит на отрезки (в веществ. числах), где func определена
std::vector<Segment> domain_segments (const std::function<double(double)>& calc, double min_x,
                                      double max_x, double max_y, double precision);

// по факту эта функция используется и в бэке и во фронте
// связано это с тем, что
// 1) она работает с веществ. числами, а значит, ей место в бэке
// 2) раньше она была идентично написана в двух местах, пусть лучше будет одна там, где ей больше
// место

}  // namespace Math_calc