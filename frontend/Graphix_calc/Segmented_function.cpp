// header
#include "Segmented_function.h"

// servant
#include "../servant/constants.h"

namespace Graphix_calc {

Segmented_function::Segmented_function(string str, int scale, Point center, int max_x, int max_y)
    : expr{Backend::function_string(str)}, segs{segments(max_x, max_y, scale)},
      seg_func{segmented_function(scale, center, max_x)},
      has_var{str.find('x') != std::string::npos}
{
}

vector<Segment> Segmented_function::domain_segments(double l_border, double r_border, double height,
                                                    double precision) const
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
                double y = expr.calculate(x);
                if (((-height / 2) < y) && (y < height / 2))
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
                double y = expr.calculate(x);
                if ((y > height / 2) or (y < -height / 2))
                {
                    seg.stop = x - precision;
                    x_is_started = false;
                    if (seg.start < seg.stop)
                        res.push_back(seg);
                }
            }
            catch (const std::exception& e)
            {
                seg.stop = x - precision;
                x_is_started = false;
                if (seg.start < seg.stop)
                    res.push_back(seg);
            }
        }
    }
    if (x_is_started)
    {
        seg.stop = r_border;
        if (seg.start < seg.stop)
            res.push_back(seg);
    }
    return res;
}

vector<Segment> Segmented_function::segments(int max_x, int max_y, int scale) const
{
    double prec_seg = (((double)max_x / sqrt(scale))) / (abs(Front_consts::max_scale - scale));
    vector<Segment> res =
        domain_segments(-((double)max_x / (2 * scale)), ((double)max_x / (2 * scale)),
                        (3 * (double)max_y / (2 * scale)), prec_seg);
    return res;
}

Graph_lib::Vector_ref<Graph_lib::Function>
Segmented_function::segmented_function(int scale, Point center, int max_x) const
{
    Graph_lib::Vector_ref<Graph_lib::Function> res;
    int point_amount = has_var ? 4 * max_x * sqrt(scale) / 3 : 500;
    for (auto seg : segs)
    {
        Graph_lib::Function* f1 =
            new Graph_lib::Function{expr.calculate, seg.start,     seg.stop,     center,
                                    point_amount,   double(scale), double(scale)};
        res.push_back(*f1);
    }
    return res;
}

Graph_lib::Vector_ref<Graph_lib::Function> Segmented_function::get_segmented_function() const
{
    return seg_func;
}

}  // namespace Graphix_calc
