#pragma once

// std libs
#include <vector>

// Math_calc
#include "math_base.h"

// Backend
#include "../function_string.h"

using std::vector;

namespace Math_calc {

class function_roots
{
  public:
    function_roots();
    function_roots(string str, double l_border, double r_border, double h_border, double precision);
    vector<Point> get_function_roots () const;

  protected:
    // деление на сегменты, где функция определена
    vector<Segment> domain_segments (double l_border, double r_border, double h_border) const;
    // деление сегмента на на подсегменты, где могут быть корни
    vector<Segment> estimated_segment (Segment seg) const;
    // нахождение корня на интервале
    double solution_on_interval (Segment seg) const;
    // нахождение всех корней
    vector<Point> solutions (double l_border, double r_border, double h_border) const;

  private:
    Backend::function_string func;
    double precision;
    vector<Point> points;
};

}  // namespace Math_calc