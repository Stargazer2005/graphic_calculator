#pragma once

// std libs
#include <string>
#include <vector>

// Math_calc
#include "math_base.h"

// Backend
#include "../function.h"

namespace Math_calc {

// класс, хранящий в себе вектор точек - пересечений графика с осями (график задан строкой)
class function_roots
{
  public:
    function_roots();  // этот конструктор нужен только для наследования
    function_roots(Backend::function _func, double l_border, double r_border, double h_border,
                   double _precision);

    // methods

    std::vector<Point> get_function_roots () const { return points; }

  protected:
    // methods

    // деление сегмента на на подсегменты, где могут быть корни (используя тот факт, что по разные
    // стороны от точки-корня функция имеет разные знаки, так как он лежит на y = 0)
    std::vector<Segment> estimated_segment (Segment seg) const;
    // нахождение корня на интервале (используя метод золотого сечения)
    double solution_on_interval (Segment seg) const;

  private:
    // variables

    // точность (используем везде, поэтому сохраняем в качестве поля)
    double precision;
    // введенная функция
    Backend::function func;
    // вектор точек, который и будем возвращать
    std::vector<Point> points;

    // methods

    // нахождение всех корней
    std::vector<Point> solutions (double l_border, double r_border, double h_border) const;
};

}  // namespace Math_calc