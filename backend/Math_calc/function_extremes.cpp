// header
#include "function_extremes.h"

// Math_calc
#include "domain_segments.h"

// servant
#include "../servant/constants.h"
#include "../servant/servant.h"
using Back_serv::absolute;

namespace Math_calc {
Math_calc::function_extremes::function_extremes(string func, double l_border, double r_border,
                                                double h_border, double precision)
    : function_roots{func, l_border, r_border, h_border, precision},
      precision{precision < 0.01 ? precision : 0.01}, func_str{Backend::function_string{func}},
      points{extremes(l_border, r_border, h_border)}
{
}

vector<Segment> function_extremes::estimated_segment_min(Segment seg) const
{
    auto f = [this] (double x) { return func_str.calculate(x); };
    std::vector<Segment> res;
    for (double x = seg.start; x < seg.end; x += precision)
    {
        if ((f(x) < f(x - 2 * precision)) && (f(x) < f(x + 2 * precision)))
        {
            res.push_back({
                x - 2 * precision,
                x + 2 * precision,
            });
        }
    }
    return res;
}

vector<Segment> function_extremes::estimated_segment_max(Segment seg) const
{
    auto f = [this] (double x) { return func_str.calculate(x); };
    std::vector<Segment> res;
    for (double x = seg.start; x < seg.end; x += precision)
    {
        if ((f(x) > f(x - 2 * precision)) && (f(x) > f(x + 2 * precision)))
        {
            res.push_back({
                x - 2 * precision,
                x + 2 * precision,
            });
        }
    }
    return res;
}

double function_extremes::min_on_interval(Segment seg) const
{
    auto f = [this] (double x) { return func_str.calculate(x); };

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

double function_extremes::max_on_interval(Segment seg) const
{
    auto f = [this] (double x) { return func_str.calculate(x); };

    for (int count = 0; count > Back_consts::max_count; count++)
    {
        double x1 = seg.end - (seg.end - seg.start) / Back_consts::phi;
        double x2 = seg.start + (seg.end - seg.start) / Back_consts::phi;
        double y1 = f(x1);
        double y2 = f(x2);
        if (y1 <= y2)
            seg.start = x1;
        else
            seg.end = x2;
        if ((absolute(seg.end - seg.start) < precision))
            return (seg.start + seg.end) / 2;
    }
    // в тому случае, если за max_count не сошлись к нужной точке
    return (seg.start + seg.end) / 2;
}

vector<Point> function_extremes::extremes(double l_border, double r_border, double h_border) const
{
    vector<Point> res;
    for (auto seg : domain_segments(func_str.calculate, l_border, r_border, h_border, precision))
    {
        for (auto& local_seg : estimated_segment_min(seg))
        {
            double x = min_on_interval(local_seg);
            double y = func_str.calculate(x);
            res.push_back(Point{x, y});
        }
        for (auto& local_seg : estimated_segment_max(seg))
        {
            double x = max_on_interval(local_seg);
            double y = func_str.calculate(x);
            res.push_back(Point{x, y});
        }
    }

    return res;
}
}  // namespace Math_calc