#include "function_crosses.h"

// std libs
using std::pair;
using std::string;
using std::vector;

// Math_calc
#include "domain_segments.h"

// servant
#include "../servant/servant.h"
using Back_serv::absolute;  // (при использовании std::abs происходит неявное преобразование)

namespace Math_calc {

function_crosses::function_crosses(std::pair<Math_func::function, Math_func::function> funcs,
                                   double l_border, double r_border, double h_border,
                                   double _precision)
    // рассматриваем иксы пересечения функций, как точки-корни для функции - их разницы
    : function_roots{Math_func::function{funcs.first.get_func_str() + "-(" +
                                         funcs.second.get_func_str() + ")"},
                     l_border, r_border, h_border, _precision},
      // FIXME: сейчас тут есть проверка на точность, которой быть не должно
      precision{_precision < 0.01 ? _precision : 0.01}, f{funcs.first},
      points{crosses(l_border, r_border, h_border)}
{
}

vector<Point> function_crosses::crosses(double l_border, double r_border, double h_border) const
{
    vector<Point> res;
    for (auto seg : domain_segments(f.calculate, l_border, r_border, h_border, precision))
    {
        for (auto& local_seg : estimated_segment(seg))
        {
            double x = function_roots::solution_on_interval(local_seg);
            double y = f(x);
            // отличие от нахождения корней заключается в том, что теперь нам не нужна проверка на
            // приближенность нулю
            res.push_back(Point{x, y});
        }
    }
    return res;
}

}  // namespace Math_calc