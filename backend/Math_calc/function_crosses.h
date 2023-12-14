#pragma once

// std libs
#include <utility>
using std::pair;

// Math_calc
#include "domain_segments.h"
#include "function_roots.h"

namespace Math_calc {

// класс, хранящий в себе вектор точек - пересечений двух графиков заданных строками
class function_crosses : public function_roots
{
  public:
    function_crosses(pair<string, string> strs, double l_border, double r_border, double h_border,
                     double precision);

    // methods

    vector<Point> get_functions_crosses () const { return points; }

  private:
    // variables

    // точность (используем везде, поэтому сохраняем в качестве поля)
    double precision;
    // одна из введенных функций (одна, так как нужно просто найти y)
    Backend::function_string func;
    // вектор точек, который и будем возвращать
    vector<Point> points;

    // methods

    // нахождение всех пересечений
    vector<Point> crosses (double l_border, double r_border, double h_border) const;
};

}  // namespace Math_calc