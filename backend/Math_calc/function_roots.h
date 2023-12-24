#pragma once

// std libs
#include <string>
#include <vector>

// Math_calc
#include "math_base.h"

// Math_function
#include "../Math_func/function.h"

namespace Math_calc {

// MEANS: класс, хранящий в себе вектор точек - пересечений графика мат. функции с осью Ox
class function_roots
{
  public:
    function_roots();  // (этот конструктор нужен только для наследования)
    function_roots(Math_func::function _func, double min_x, double max_x, double max_y,
                   double _precision);

    // methods

    inline std::vector<Point> get_function_roots () const { return points; }

    // ~methods

  protected:
    // methods

    // DOES: делит сегмент на на подсегменты, где могут быть корни
    // (используя тот факт, что по разные стороны от точки-корня мат. функция имеет разные знаки)
    std::vector<Segment> estimated_segment(Segment) const;

    // DOES: находит корень на интервале, используя метод золотого сечения
    double root_on_interval(Segment) const;

    // ~methods

  private:
    // vars

    // MEANS: точность (используем везде, поэтому сохраняем в качестве поля)
    double precision;

    // MEANS: введенная мат. функция
    Math_func::function f;

    std::vector<Point> points;

    // ~vars

    // methods

    // DOES: находит все корни перебором по сегментам
    std::vector<Point> roots (double min_x, double max_x, double max_y) const;

    // ~methods
};

}  // namespace Math_calc