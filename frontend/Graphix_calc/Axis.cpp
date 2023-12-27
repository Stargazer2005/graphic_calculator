
#include "Axis.h"

// std libs
#include <stdexcept>
using std::invalid_argument;
using std::string;

// Graph_lib
using Graph_lib::Color;
using Graph_lib::Point;
using Graph_lib::Text;

// utility
#include "../utility/constants.h"
#include "../utility/utilities.h"
using namespace Frontend_consts;
using namespace Frontend_utilities;

namespace Graphix_calc {

Axis::Axis(Orientation d, Point origin, pix_amount length, pix_amount _unit_intr,
           const std::string& label_text)
    : unit_intr{_unit_intr}, mark_intr{_unit_intr},
      label{Point{origin.x - margin, origin.y - length / 2 + margin}, label_text}
{
    switch (d)
    {
    case Axis::horisontal:
    {
        Shape::add(origin);
        Shape::add(Point{origin.x - length / 2, origin.y});
        Shape::add(Point{origin.x + length / 2, origin.y});

        for (int i = 1; i < length / 2; ++i)
        {
            if (i % unit_intr == 0)
            {
                notches.add(Point{origin.x + i, origin.y + 2}, Point{origin.x + i, origin.y - 2});
                notches.add(Point{origin.x - i, origin.y + 2}, Point{origin.x - i, origin.y - 2});
            }

            if (i % mark_intr == 0)
            {
                Text* mark = new Text(
                    Point(origin.x + i - 3, origin.y + 20),
                    format(convert_to_real(origin, {origin.x + i, origin.y + 20}, unit_intr).x));
                mark->set_color(Graph_lib::Color::black);
                mark->set_font_size(14);
                marks.push_back(mark);

                mark = new Text(
                    Point(origin.x - i - 6, origin.y + 20),
                    format(convert_to_real(origin, {origin.x - i, origin.y + 20}, unit_intr).x));
                mark->set_color(Graph_lib::Color::black);
                mark->set_font_size(14);
                marks.push_back(mark);
            }
        }
        break;
    }
    case Axis::vertical:
    {
        Shape::add(Point{origin.x, origin.y + length / 2});
        Shape::add(origin);
        Shape::add(Point{origin.x, origin.y - length / 2});

        for (int i = 1; i < length / 2; ++i)
        {
            if (i % unit_intr == 0)
            {
                notches.add(Point{origin.x + 2, origin.y + i}, Point{origin.x - 2, origin.y + i});
                notches.add(Point{origin.x + 2, origin.y - i}, Point{origin.x - 2, origin.y - i});
            }

            if (i % mark_intr == 0)
            {
                Text* mark = new Text(
                    Point(origin.x + 20, origin.y + i + 3),
                    format(convert_to_real(origin, {origin.x + 20, origin.y + i}, unit_intr).y));
                mark->set_color(Graph_lib::Color::black);
                mark->set_font_size(14);
                marks.push_back(mark);

                mark = new Text(
                    Point(origin.x + 20, origin.y - i + 3),
                    format(convert_to_real(origin, {origin.x + 20, origin.y - i}, unit_intr).y));
                mark->set_color(Graph_lib::Color::black);
                mark->set_font_size(14);
                marks.push_back(mark);
            }
        }
        break;
    }
    }
}

Axis::Axis(Orientation d, Graph_lib::Point origin, pix_amount length, pix_amount _unit_intr,
           pix_amount _mark_intr, const std::string& label_text)
    : unit_intr{_unit_intr}, mark_intr{_mark_intr},
      label{Point{origin.x - margin, origin.y - length / 2 + margin}, label_text}
{
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