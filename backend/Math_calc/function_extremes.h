#pragma once

// std libs
#include <string>
#include <vector>

// Math_calc
#include "function_roots.h"
#include "math_base.h"

namespace Math_calc {

// MEANS: класс, хранящий в себе вектор точек - локальные экстремумы мат. функции
class function_extremes : public function_roots
{
  public:
    function_extremes(Math_func::function _func, double min_x, double max_x, double max_y,
                      double _precision);

    // methods

    inline std::vector<Point> get_function_extremes () const { return points; }

    // ~methods

  private:
    enum TypeExtreme
    {
        pnt_min,
        pnt_max
    };

    // vars

    // MEANS: точность
    // (используем везде, поэтому сохраняем в качестве поля)
    double precision;

    // MEANS: введенная мат. функция
    Math_func::function f;

    std::vector<Point> points;

    // ~vars

    // methods

    // DOES: делит сегменты на подсегменты, где могут быть экстремумы
    std::vector<Segment> estimated_segment(TypeExtreme, Segment) const;

    // DOES: находит экстремум на интервале, используя метод золотого сечения
    double extreme_on_interval(TypeExtreme, Segment) const;

    // DOES: находит все локальные экстремумы перебором по сегментам
    std::vector<Point> extremes (double min_x, double max_x, double max_y) const;

    // ~methods
};

}  // namespace Math_calc