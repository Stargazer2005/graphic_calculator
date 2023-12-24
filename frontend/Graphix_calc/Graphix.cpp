
#include "Graphix.h"

// std libs
#include <stdexcept>
using std::invalid_argument;
using std::function;

namespace Graphix_calc {

Graphix::Graphix(function<double(double)> func, double min_x, double max_x,
                 Graph_lib::Point center, int point_amount, double scale)
{
    auto convert_to_pix = [&] (double x, double y) -> Graph_lib::Point
    {
        return Graph_lib::Point{center.x + static_cast<int>(x * scale),
                                center.y - static_cast<int>(y * scale)};
    };

    if (max_x - min_x <= 0)
        invalid_argument("bad graphing range");

    if (point_amount <= 0)
        invalid_argument("non-positive graphing count");
    else if (point_amount == 2)
    {
        add_point(convert_to_pix(min_x, func(min_x)));
        add_point(convert_to_pix(max_x, func(max_x)));
    }
    else
    {
        double dist = (max_x - min_x) / point_amount;
        double r = min_x;
        for (int i = 0; i < point_amount; ++i)
        {
            add_point(Graph_lib::Point{center.x + int(r * scale), center.y - int(func(r) * scale)});
            r += dist;
        }
    }
}

void Graphix::add_point(Graph_lib::Point p)
{
    add(p);
    points.push_back(p);
}

}  // namespace Graphix_calc
