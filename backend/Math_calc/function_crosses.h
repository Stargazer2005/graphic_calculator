#pragma once

// Math_calc
#include "function_roots.h"

#include <utility>

using std::pair;

namespace Math_calc {

class function_crosses : public function_roots
{
  public:
    function_crosses(pair<string, string> strs, double l_border, double r_border, double h_border,
                     double precision);
    vector<Point> get_functions_crosses () const;

  private:
    // нахождение всех пересечений
    vector<Point> crosses (double l_border, double r_border, double h_border) const;
    double precision;
    Backend::function_string func;
    vector<Point> points;
};

}  // namespace Math_calc