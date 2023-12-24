#pragma once

// std libs
#include <string>
#include <utility>
#include <vector>

// Math_calc
#include "function_roots.h"
#include "math_base.h"

namespace Math_calc {

// MEMO: класс, хранящий в себе вектор точек - пересечений двух func'ов
class function_crosses : public function_roots
{
  public:
    function_crosses(std::pair<Math_func::function, Math_func::function> funcs, double l_border,
                     double r_border, double h_border, double _precision);

    // methods

    std::vector<Point> get_functions_crosses () const { return points; }

  private:
    // vars

    // MEMO: точность
    // (используем везде, поэтому сохраняем в качестве поля)
    double precision;
    // MEMO: одна из введенных функций
    // (одна, так как нужно просто найти y, можно и в одну подставить)
    Math_func::function f;
    std::vector<Point> points;

    // methods

    // MEMO: нахождение всех пересечений
    std::vector<Point> crosses (double l_border, double r_border, double h_border)
        const;  // (чтобы не хранить границы внутри класса, передаём их в аргументы)
};

}  // namespace Math_calc