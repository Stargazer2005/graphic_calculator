#pragma once

// Math_calc
#include "function_roots.h"

namespace Math_calc {
class function_extremes : public function_roots
{
  public:
    function_extremes(string func, double l_border, double r_border, double h_border,
                      double precision);

    vector<Point> get_function_extremes () const { return points; }

  private:
    // methods

    // деление сегмента на на подсегменты, где могут быть экстремумы (используя метод золотого
    // сечения)
    vector<Segment> estimated_segment_min (Segment seg) const;
    vector<Segment> estimated_segment_max (Segment seg) const;
    // нахождение экстремума на интервале
    double min_on_interval (Segment seg) const;
    double max_on_interval (Segment seg) const;

    // variables

    // точность (используем везде, поэтому сохраняем в качестве поля)
    double precision;
    //
    Backend::function_string func_str;
    // вектор точек, который и будем возвращать
    vector<Point> points;

    // methods

    // нахождение всех пересечений
    vector<Point> extremes (double l_border, double r_border, double h_border) const;
};
}  // namespace Math_calc