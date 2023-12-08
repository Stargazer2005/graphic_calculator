#include "function_string.h"
#include <cmath>

vector<Backend::Segment> Backend::function_string::domain_segments(double l_border, double r_border,
                                                                   double height,
                                                                   double precision) const
{
    // std::cout << "IN  ->"
    //           << "Get_info" << std::endl;
    std::vector<Segment> res;
    bool x_is_started = false;
    Segment seg;
    // std::cout << left_b << " " << right_b << " " << precision << std::endl;
    for (double x = l_border; x < r_border; x += precision)
    {
        if (!x_is_started)
        {
            try
            {
                double y = calc(x);
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
                double y = calc(x);
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
    // std::cout << "OUT<- "
    //           << "get_info" << std::endl;
    return res;
}

void Backend::function_string::set_segments(int max_x, int max_y, int scale)
{
    double prec_seg = (ceil(max_x / (2 * scale)) + 1) / 100000;
    segments = domain_segments(-ceil(max_x / (2 * scale)) - 1, ceil(max_x / (2 * scale)) + 1,
                               (2 * max_y / scale) + 1, prec_seg);
}

vector<Backend::Segment> Backend::function_string::get_segments() { return segments; }
