// header
#include "function_roots.h"

// std libs
#include <cmath>

// servant
#include "../servant/constants.h"
#include "../servant/servant.h"

using Back_serv::absolute;

namespace Math_calc {

function_roots::function_roots()
    : precision{0}, func{Backend::function_string("0")}, points{vector<Point>{}}
{
}

function_roots::function_roots(string str, double l_border, double r_border, double h_border,
                               double precision)
    : precision{precision < 0.01 ? precision : 0.01}, func{Backend::function_string(str)},
      points{solutions(l_border, r_border, h_border)}

{
}

vector<Point> function_roots::get_function_roots() const { return points; }

vector<Segment> function_roots::domain_segments(double l_border, double r_border,
                                                double h_border) const
{
    std::vector<Segment> res;
    bool x_is_started = false;
    Segment seg;
    for (double x = l_border; x < r_border; x += precision)
    {
        if (!x_is_started)
        {
            try
            {
                double y = func.calculate(x);
                if (((-h_border / 2) < y) && (y < h_border / 2))
                {
                    seg.start = x;
                    x_is_started = true;
                }
            }
            catch (const std::exception& e)
            {
            }
        }
        else
        {
            try
            {
                double y = func.calculate(x);
                if ((y > h_border / 2) or (y < -h_border / 2))
                {
                    seg.end = x - precision;
                    x_is_started = false;
                    if (seg.start < seg.end)
                        res.push_back(seg);
                }
            }
            catch (const std::exception& e)
            {
                seg.end = x - precision;
                x_is_started = false;
                if (seg.start < seg.end)
                    res.push_back(seg);
            }
        }
    }
    if (x_is_started)
    {
        seg.end = r_border;
        if (seg.start < seg.end)
            res.push_back(seg);
    }
    return res;
}

vector<Segment> function_roots::estimated_segment(Segment seg) const
{
    std::vector<Segment> res;
    for (double x = seg.start; x < seg.end; x += precision)
    {
        if (func.calculate(x) * func.calculate(x - precision) <= 0)
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
    auto f = [this] (double x) { return pow(func.calculate(x), 2); };
    int count = 0;
    for (;;)
    {
        double x1 = seg.end - (seg.end - seg.start) / Back_consts::phi;
        double x2 = seg.start + (seg.end - seg.start) / Back_consts::phi;
        double y1 = f(x1);
        double y2 = f(x2);
        if (y1 >= y2)
            seg.start = x1;
        else
            seg.end = x2;
        if ((absolute(seg.end - seg.start) < precision) || (count > 10000))
        {
            std::cout << "prec" << precision << " " << count << std::endl;
            return (seg.start + seg.end) / 2;
        }
        count += 1;
    }
}

vector<Point> function_roots::solutions(double l_border, double r_border, double h_border) const
{
    vector<Point> res;
    for (auto seg : domain_segments(l_border, r_border, h_border))
    {
        for (auto& local_seg : estimated_segment(seg))
        {
            double x = solution_on_interval(local_seg);
            double y = func.calculate(x);
            if (absolute(y) < precision * 20)
            {
                // std::cout << x << " " << y << std::endl;
                res.push_back(Point{x, 0});
            }
        }
    }
    return res;
}

}  // namespace Math_calc