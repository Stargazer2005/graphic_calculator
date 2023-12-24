#pragma once

// std libs
#include <string>
#include <vector>

// Math_calc
#include "math_base.h"

// Math_function
#include "../Math_func/function.h"

namespace Math_calc {

// MEMO: класс, хранящий в себе вектор точек - пересечений графика func с осью Ox
class function_roots
{
  public:
    function_roots();  // (этот конструктор нужен только для наследования)
    function_roots(Math_func::function _func, double l_border, double r_border, double h_border,
                   double _precision);

    // methods

    std::vector<Point> get_function_roots () const { return points; }

  protected:
    // methods

    // MEMO: деление сегмента на на подсегменты, где могут быть корни
    // (используя тот факт, что по разные стороны от точки-корня func имеет разные знаки)
    std::vector<Segment> estimated_segment(Segment) const;

    // MEMO: нахождение корня на интервале (используя метод золотого сечения)
    double solution_on_interval(Segment) const;

  private:
    // vars

    // MEMO: точность (используем везде, поэтому сохраняем в качестве поля)
    double precision;
    // MEMO: введенная func
    Math_func::function f;
    std::vector<Point> points;

    // methods

    // MEMO: нахождение всех корней
    std::vector<Point> roots (double l_border, double r_border, double h_border) const;
};

}  // namespace Math_calc