// header
#include "domain_segments.h"

namespace Math_calc {

vector<Segment> domain_segments (const function<double(double)>& func, double l_border,
                                 double r_border, double height, double precision)
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
                double y = func(x);
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
                double y = func(x);
                if ((y > height / 2) or (y < -height / 2))
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

}  // namespace Math_calc