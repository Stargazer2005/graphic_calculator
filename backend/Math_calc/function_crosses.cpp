// header
#include "function_crosses.h"

// std libs
#include <cmath>
#include <iostream>

// servant
#include "../servant/constants.h"
#include "../servant/servant.h"

using Back_serv::absolute;

namespace Math_calc {

function_crosses::function_crosses(pair<string, string> strs, double l_border, double r_border,
                                   double h_border, double precision)
    : function_roots{strs.first + "-(" + strs.second + ")", l_border, r_border, h_border,
                     precision},
      precision{precision < 0.01 ? precision : 0.01}, func{Backend::function_string{strs.first}},
      points{crosses(l_border, r_border, h_border)}

{
}

vector<Point> function_crosses::get_functions_crosses() const { return points; }

vector<Point> function_crosses::crosses(double l_border, double r_border, double h_border) const
{
    vector<Point> res;
    for (auto seg : domain_segments(l_border, r_border, h_border))
    {
        for (auto& local_seg : estimated_segment(seg))
        {
            double x = solution_on_interval(local_seg);
            double y = func.calculate(x);
            res.push_back(Point{x, y});
        }
    }
    return res;
}

}  // namespace Math_calc