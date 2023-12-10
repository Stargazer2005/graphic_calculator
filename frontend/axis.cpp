#include "axis.h"
#include "servant/constants.h"

using namespace Graph_lib;
using namespace Front_consts;

namespace Graphix_calc {

Axis::Axis(Orientation d, Point xy, pix_numb length, pix_numb scale, const std::string& lab)
    : dist{scale}, label{Point{0, 0}, lab}
{
    if (length < 0)
        error("bad axis length");
    switch (d)
    {
    case Axis::x:
    {
        Shape::add(Point{xy.x - length / 2, xy.y});
        Shape::add(xy);
        Shape::add(Point{xy.x + length / 2, xy.y});
        if (dist > 0)
        {
            // счетчики, с помощью которых мы ставим насечки на осях
            pix_numb x1 = xy.x + dist;
            pix_numb x2 = xy.x - dist;
            for (pix_numb i = 0; i < length / (2 * dist); ++i)
            {
                notches.add(Point{x1, xy.y + 2}, Point{x1, xy.y - 2});
                x1 += dist;
                notches.add(Point{x2, xy.y + 2}, Point{x2, xy.y - 2});
                x2 -= dist;
            }
        }
        // label under the line
        label.move(length - margin, xy.y + margin);
        break;
    }
    case Axis::y:
    {
        Shape::add(Point{xy.x, xy.y + length / 2});
        Shape::add(xy);
        Shape::add(Point{xy.x, xy.y - length / 2});
        if (dist > 0)
        {
            // счетчики, с помощью которых мы ставим насечки на осях
            pix_numb y1 = xy.y + dist;
            pix_numb y2 = xy.y - dist;
            for (pix_numb i = 0; i < length / (2 * dist); ++i)
            {
                notches.add(Point{xy.x - 2, y1}, Point{xy.x + 2, y1});
                y1 += dist;
                notches.add(Point{xy.x - 2, y2}, Point{xy.x + 2, y2});
                y2 -= dist;
            }
        }
        // label at top
        label.move(xy.x - margin, xy.y - length / 2 + margin);
        break;
    }
    }
}

void Axis::draw_lines() const
{
    Shape::draw_lines();
    notches.draw();
    label.draw();
}

void Axis::set_color(Color c)
{
    Shape::set_color(c);
    notches.set_color(c);
    label.set_color(c);
}

}  // namespace Graphix_calc