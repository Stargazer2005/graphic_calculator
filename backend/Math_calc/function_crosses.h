#pragma once

// std libs
#include <string>
#include <utility>
#include <vector>

// Math_calc
#include "function_roots.h"
#include "math_base.h"

namespace Math_calc {

// класс, хранящий в себе вектор точек - пересечений двух графиков заданных строками
class function_crosses : public function_roots
{
  public:
    function_crosses(std::pair<std::string, std::string> funcs, double l_border, double r_border,
                     double h_border, double precision);

    // methods

    std::vector<Point> get_functions_crosses () const { return points; }

  private:
    // variables

    // точность (используем везде, поэтому сохраняем в качестве поля)
    double precision;
    // одна из введенных функций (одна, так как нужно просто найти y, можно и в одну подставить)
    Backend::math_function func_str;
    // вектор точек, который и будем возвращать
    std::vector<Point> points;

    // methods

    // нахождение всех пересечений
    std::vector<Point> crosses (double l_border, double r_border, double h_border) const;
};

}  // namespace Math_calc