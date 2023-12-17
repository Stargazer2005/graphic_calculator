// header
#include "Graphix.h"

// std libs
#include <stdexcept>
using std::invalid_argument;

namespace Graphix_calc {

Graphix::Graphix(function<double(double)> func, double l_border, double r_border,
                 Graph_lib::Point center, int point_amount, int scale)
{
    auto convert_to_pix = [&] (double x, double y) -> Graph_lib::Point
    {
        return Graph_lib::Point{center.x + static_cast<int>(x * scale),
                                center.y - static_cast<int>(y * scale)};
    };

    if (r_border - l_border <= 0)
        invalid_argument("bad graphing range");
    if (point_amount <= 0)
        invalid_argument("non-positive graphing count");
    if (point_amount == 2)
    {
        add(convert_to_pix(l_border, func(l_border)));
        add(convert_to_pix(r_border, func(r_border)));
    }
    else
    {
        double dist = (r_border - l_border) / point_amount;
        double r = l_border;
        for (int i = 0; i < point_amount; ++i)
        {
            add(Point{center.x + int(r * scale), center.y - int(func(r) * scale)});
            r += dist;
        }
    }
}

}  // namespace Graphix_calc
