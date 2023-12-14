#pragma once

// std libs
#include <vector>
using std::vector;

// Backend
#include "../function_string.h"
using Backend::function_string;

// Math_calc
#include "math_base.h"

namespace Math_calc {

// функция, которая дробит на отрезки (в рациональных числах), где функция определена
vector<Segment> domain_segments (function_string func, double l_border, double r_border,
                                 double height, double precision);

// по факту сейчас эта функция используется и в бэке и во фронте
// связано это с тем, что
// 1) она работает с рациональными числами, а значит, ей место в бэке
// 2) раньше она была идентично написана в двух местах, пусть лучше будет одна там, где ей больше
// места :)

}  // namespace Math_calc