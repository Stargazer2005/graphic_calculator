// header
#include "function_crosses.h"

// servant
#include "../servant/constants.h"
#include "../servant/servant.h"
using Back_serv::absolute;

namespace Math_calc {

function_crosses::function_crosses(pair<string, string> funcs, double l_border, double r_border,
                                   double h_border, double precision)
    : function_roots{funcs.first + "-(" + funcs.second + ")", l_border, r_border, h_border,
                     precision},
      precision{precision < 0.01 ? precision : 0.01},
      func_str{Backend::function_string{funcs.first}}, points{crosses(l_border, r_border, h_border)}

{
}

vector<Point> function_crosses::crosses(double l_border, double r_border, double h_border) const
{
  vector<Point> res;
  for (auto seg : domain_segments(func_str.calculate, l_border, r_border, h_border, precision))
  {
    for (auto& local_seg : estimated_segment(seg))
    {
      double x = solution_on_interval(local_seg);
      double y = func_str.calculate(x);
      res.push_back(Point{x, y});
    }
  }
  return res;
}

}  // namespace Math_calc