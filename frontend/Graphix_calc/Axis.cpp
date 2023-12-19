// header
#include "Axis.h"

// std libs
#include <stdexcept>
using std::invalid_argument;
using std::string;

// Graph_lib
#include <Graph_lib/Graph.h>
using Graph_lib::Color, Graph_lib::Point;

// servant
#include "../servant/constants.h"
using namespace Front_consts;

namespace Graphix_calc {

Axis::Axis(Orientation d, Point center, int length, int scale, const std::string& lab)
    : dist{scale}, label{Point{0, 0}, lab}
{
    if (length < 0 || dist < 0)
        throw invalid_argument("bad axis length");
    switch (d)
    {
    case Axis::horisontal:
    {
        Shape::add(Point{center.x - length / 2, center.y});
        Shape::add(center);
        Shape::add(Point{center.x + length / 2, center.y});
        // счетчики, с помощью которых мы ставим насечки на осях
        int x1 = center.x + dist;
        int x2 = center.x - dist;
        for (int i = 0; i < length / (2 * dist); ++i)
        {
            notches.add(Point{x1, center.y + 2}, Point{x1, center.y - 2});
            x1 += dist;
            notches.add(Point{x2, center.y + 2}, Point{x2, center.y - 2});
            x2 -= dist;
        }
        // label under the line
        label.move(center.x + length / 2 - margin, center.y + margin);
        break;
    }
    case Axis::vertical:
    {
        Shape::add(Point{center.x, center.y + length / 2});
        Shape::add(center);
        Shape::add(Point{center.x, center.y - length / 2});
        // счетчики, с помощью которых мы ставим насечки на осях
        int y1 = center.y + dist;
        int y2 = center.y - dist;
        for (int i = 0; i < length / (2 * dist); ++i)
        {
            notches.add(Point{center.x - 2, y1}, Point{center.x + 2, y1});
            y1 += dist;
            notches.add(Point{center.x - 2, y2}, Point{center.x + 2, y2});
            y2 -= dist;
        }
        // label at top
        label.move(center.x - margin, center.y - length / 2 + margin);
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