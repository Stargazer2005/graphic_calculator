#pragma once

// std libs
#include <string>
#include <utility>
#include <vector>

// Math_calc
#include "function_roots.h"
#include "math_base.h"

namespace Math_calc {

// MEANS: класс, хранящий в себе вектор точек - пересечений двух мат. функций
class function_crosses : public function_roots
{
  public:
    function_crosses(std::pair<Math_func::function, Math_func::function> funcs, double min_x,
                     double max_x, double max_y, double _precision);

    // methods

    inline std::vector<Point> get_functions_crosses () const { return points; }

    // ~methods

  private:
    // vars

    // MEANS: точность
    // (используем везде, поэтому сохраняем в качестве поля)
    double precision;

    // MEANS: одна из введенных мат. функций
    // (одна, так как нужно просто найти y, можно и в одну подставить)
    Math_func::function f;

    std::vector<Point> points;

    // ~vars

    // methods

    // RETURNS: все пересечения двух мат. функций (точки: пары вида (x,y))
    // (по известным макс., мин. значениям)
    std::vector<Point> crosses (double min_x, double max_x, double max_y)
        const;  // (чтобы не хранить границы внутри класса, передаём их в аргументы)

    // ~methods
};

}  // namespace Math_calc