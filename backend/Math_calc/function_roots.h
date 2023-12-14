#pragma once

// std libs
#include <vector>
using std::vector;

// Math_calc
#include "domain_segments.h"
#include "math_base.h"

// Backend
#include "../function_string.h"
using Backend::function_string;

namespace Math_calc {

// класс, хранящий в себе вектор точек - пересечений графика с осями (график задан строкой)
class function_roots
{
  public:
    function_roots();
    function_roots(string str, double l_border, double r_border, double h_border, double precision);

    // methods

    vector<Point> get_function_roots () const { return points; }

  protected:
    // methods

    // деление сегмента на на подсегменты, где могут быть корни (используя метод золотого сечения)
    vector<Segment> estimated_segment (Segment seg) const;
    // нахождение корня на интервале
    double solution_on_interval (Segment seg) const;

  private:
    // variables

    // точность (используем везде, поэтому сохраняем в качестве поля)
    double precision;
    // введенная функция
    function_string func;
    // вектор точек, который и будем возвращать
    vector<Point> points;

    // methods

    // нахождение всех корней
    vector<Point> solutions (double l_border, double r_border, double h_border) const;
};

}  // namespace Math_calc