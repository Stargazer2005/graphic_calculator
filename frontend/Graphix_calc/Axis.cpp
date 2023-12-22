// header
#include "Axis.h"

// std libs
#include <stdexcept>
using std::invalid_argument;
using std::string;

// Graph_lib
using Graph_lib::Color;
using Graph_lib::Point;
using Graph_lib::Text;

// servant
#include "../servant/constants.h"
#include "../servant/format.h"
using namespace Front_consts;

namespace Graphix_calc {

Axis::Axis(Orientation d, Point center, int length, double scale, const std::string& lab)
    : dist{distance}, label{Point{0, 0}, lab}
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
        double count = 0;
        // счетчики, с помощью которых мы ставим насечки на осях
        int x1 = center.x + dist;
        int x2 = center.x - dist;
        for (int i = 0; i < length / (2 * dist); ++i)
        {
            count += dist / scale;
            notches.add(Point{x1, center.y + 2}, Point{x1, center.y - 2});
            Text* mark1 = new Text(Point(x1 - 3, center.y + 20), format(count));
            mark1->set_color(Graph_lib::Color::black);
            mark1->set_font_size(14);
            marks.push_back(mark1);
            x1 += dist;
            notches.add(Point{x2, center.y + 2}, Point{x2, center.y - 2});
            Text* mark2 = new Text(Point(x2 - 3, center.y + 20), format(-count));
            mark2->set_color(Graph_lib::Color::black);
            mark2->set_font_size(14);
            marks.push_back(mark2);
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
        double count = 0;
        // счетчики, с помощью которых мы ставим насечки на осях
        int y1 = center.y + dist;
        int y2 = center.y - dist;
        for (int i = 0; i < length / (2 * dist); ++i)
        {
            count += (double)dist / scale;
            notches.add(Point{center.x - 2, y1}, Point{center.x + 2, y1});
            Text* mark1 = new Text(Point(center.x + 10, y1 + 5), format(-count));
            mark1->set_color(Graph_lib::Color::black);
            mark1->set_font_size(14);
            marks.push_back(mark1);
            y1 += dist;
            notches.add(Point{center.x - 2, y2}, Point{center.x + 2, y2});
            Text* mark2 = new Text(Point(center.x + 10, y2 + 5), format(count));
            mark2->set_color(Graph_lib::Color::black);
            mark2->set_font_size(14);
            marks.push_back(mark2);
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
    label.draw();
    for (const auto& mark : marks)
        mark->draw();
    notches.draw();
}

void Axis::set_color(Color c)
{
    Shape::set_color(c);
    label.set_color(c);
    notches.set_color(c);
}

}  // namespace Graphix_calc