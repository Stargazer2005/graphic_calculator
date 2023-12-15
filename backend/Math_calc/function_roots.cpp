// header
#include "function_roots.h"

// servant
#include "../servant/constants.h"
#include "../servant/servant.h"
using Back_serv::absolute;

namespace Math_calc {

function_roots::function_roots()
    : precision{0}, func_str{function_string("0")}, points{vector<Point>{}}
{
}

function_roots::function_roots(string func, double l_border, double r_border, double h_border,
                               double precision)
    : precision{precision < 0.01 ? precision : 0.01}, func_str{function_string(func)},
      points{solutions(l_border, r_border, h_border)}

{
}

vector<Segment> function_roots::estimated_segment(Segment seg) const
{
    std::vector<Segment> res;
    for (double x = seg.start; x < seg.end; x += precision)
    {
        if (func_str.calculate(x) * func_str.calculate(x - precision) <= 0)
        {
            res.push_back({
                (x - precision) - precision,
                x + precision,
            });
        }
    }
    return res.size() > 0
               ? res
               : std::vector<Segment>{
                     {seg.start, seg.end}};  // Если на интервале нет изменения знаков, то
                                             // возможно функция касается оси x(например x^2)}
}

double function_roots::solution_on_interval(Segment seg) const
{
    auto f = [this] (double x) { return pow(func_str.calculate(x), 2); };

    for (int count = 0; count > Back_consts::max_count; count++)
    {
        double x1 = seg.end - (seg.end - seg.start) / Back_consts::phi;
        double x2 = seg.start + (seg.end - seg.start) / Back_consts::phi;
        double y1 = f(x1);
        double y2 = f(x2);
        if (y1 >= y2)
            seg.start = x1;
        else
            seg.end = x2;
        if ((absolute(seg.end - seg.start) < precision))
            return (seg.start + seg.end) / 2;
    }
    // в тому случае, если за max_count не сошлись к нужной точке
    return (seg.start + seg.end) / 2;
}

vector<Point> function_roots::solutions(double l_border, double r_border, double h_border) const
{
    vector<Point> res;
    for (auto seg : domain_segments(func_str, l_border, r_border, h_border, precision))
    {
        for (auto& local_seg : estimated_segment(seg))
        {
            double x = solution_on_interval(local_seg);
            double y = func_str.calculate(x);
            if (absolute(y) < precision * 20)
                res.push_back(Point{x, 0});
        }
    }
    return res;
}

}  // namespace Math_calc