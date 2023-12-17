// header
#include "Graphix.h"

// std libs
#include <stdexcept>
using std::invalid_argument;

namespace Graphix_calc {

Graphix::Graphix(function<double(double)> func, double l_border, double r_border,
                 Graph_lib::Point center, int point_amount, int scale)
{
  if (r_border - l_border <= 0)
    invalid_argument("bad graphing range");
  if (point_amount <= 0)
    invalid_argument("non-positive graphing count");
  double dist = (r_border - l_border) / point_amount;
  double r = l_border;
  for (int i = 0; i < point_amount; ++i)
  {
    add(Point{center.x + int(r * scale), center.y - int(func(r) * scale)});
    r += dist;
  }
}

}  // namespace Graphix_calc
