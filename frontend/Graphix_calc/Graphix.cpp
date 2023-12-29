#include "Graphix.h"

// std libs
#include <stdexcept>
using std::function;
using std::invalid_argument;

// utility
#include "../utility/utilities.h"
using namespace Frontend_utilities;

namespace Graphix_calc {

Graphix::Graphix(function<double(double)> calc, double min_x, double max_x, Graph_lib::Point origin,
                 int point_amount, double unit_intr)
{
    if (max_x - min_x <= 0)
        invalid_argument("bad graphing range");

    if (point_amount <= 0)
        invalid_argument("non-positive graphing count");

    if (point_amount == 2)
    {
        add(convert_to_pix(origin, {min_x, calc(min_x)}, unit_intr));
        add(convert_to_pix(origin, {max_x, calc(max_x)}, unit_intr));
    }
    else
    {
        double dist = (max_x - min_x) / double(point_amount);
        double r = min_x;
        for (int i = 0; i < point_amount; ++i)
        {
            add(Graph_lib::Point{origin.x + int(r * unit_intr),
                                 origin.y - int(calc(r) * unit_intr)});
            r += dist;
        }
    }
}

}  // namespace Graphix_calc
