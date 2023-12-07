#include "backend.h"

using namespace Backend;

vector<Segment> graphic_string::domain_segments(double l_border, double r_border, double height,
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
