#pragma once

// std libs
#include <string>
#include <vector>

// Math_calc
#include "function_roots.h"
#include "math_base.h"

namespace Math_calc {

// MEMO: класс, хранящий в себе вектор точек - локальные экстремумы мат. функции
class function_extremes : public function_roots
{
  public:
    function_extremes(Math_func::function _func, double l_border, double r_border, double h_border,
                      double _precision);

    std::vector<Point> get_function_extremes () const { return points; }

  private:
    enum TypeExtreme
    {
        pnt_min,
        pnt_max
    };

    // vars

    // MEMO: точность
    // (используем везде, поэтому сохраняем в качестве поля)
    double precision;
    // MEMO: введенная функция
    Math_func::function f;
    std::vector<Point> points;

    // methods

    // MEMO: деление сегмента на на подсегменты, где могут быть экстремумы
    std::vector<Segment> estimated_segment(TypeExtreme, Segment) const;
    // MEMO: нахождение экстремума на интервале, используя метод золотого сечения
    double extreme_on_interval(TypeExtreme, Segment) const;

    // MEMO: нахождение всех локальных экстремумов
    std::vector<Point> extremes (double l_border, double r_border, double h_border) const;
};
}  // namespace Math_calc