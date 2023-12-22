#pragma once

// std libs
#include <string>
#include <vector>

// Math_calc
#include "function_roots.h"
#include "math_base.h"

namespace Math_calc {

class function_extremes : public function_roots
{
  public:
    function_extremes(Backend::function _func, double l_border, double r_border, double h_border,
                      double _precision);

    std::vector<Point> get_function_extremes () const { return points; }

  private:
    enum TypeExtreme
    {
        pnt_min,
        pnt_max
    };

    // variables

    // точность (используем везде, поэтому сохраняем в качестве поля)
    double precision;
    Backend::function func;
    // вектор точек, который и будем возвращать
    std::vector<Point> points;

    // methods

    // деление сегмента на на подсегменты, где могут быть экстремумы
    std::vector<Segment> estimated_segment (TypeExtreme extr, Segment seg) const;
    // нахождение экстремума на интервале (используя метод золотого сечения)
    double extreme_on_interval (TypeExtreme extr, Segment seg) const;

    // нахождение всех пересечений
    std::vector<Point> extremes (double l_border, double r_border, double h_border) const;
};
}  // namespace Math_calc