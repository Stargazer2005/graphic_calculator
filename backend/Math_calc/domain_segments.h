#pragma once

// std libs
#include <functional>
#include <vector>
using std::function, std::vector;

// Math_calc
#include "math_base.h"

namespace Math_calc {

// функция, которая дробит на отрезки (в веществ. числах), где функция определена
vector<Segment> domain_segments (const function<double(double)>& func, double l_border,
                                 double r_border, double height, double precision);

// по факту сейчас эта функция используется и в бэке и во фронте
// связано это с тем, что
// 1) она работает с веществ. числами, а значит, ей место в бэке
// 2) раньше она была идентично написана в двух местах, пусть лучше будет одна там, где ей больше
// место :)

}  // namespace Math_calc