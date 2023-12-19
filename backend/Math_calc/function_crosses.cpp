// header
#include "function_crosses.h"

// std libs
using std::pair, std::string, std::vector;

// Math_calc
#include "domain_segments.h"

// servant
#include "../servant/constants.h"
#include "../servant/servant.h"
using Back_serv::absolute;

namespace Math_calc {

function_crosses::function_crosses(pair<string, string> funcs, double l_border, double r_border,
                                   double h_border, double precision)
    // рассматриваем иксы пересечения функций, как точки-корни для функции - их разницы
    : function_roots{funcs.first + "-(" + funcs.second + ")", l_border, r_border, h_border,
                     precision},
      // FIXME: сейчас тут есть проверка на точность, которой быть не должно
      precision{precision < 0.01 ? precision : 0.01}, func_str{Backend::math_function{funcs.first}}

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
            // отличие от нахождения корней заключается в том, что теперь нам не нужна проверка на
            // приближенность нулю
            res.push_back(Point{x, y});
        }
    }
    return res;
}

}  // namespace Math_calc