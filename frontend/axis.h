#pragma once

#include "constants.h"
#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

using Graph_lib::Point;

namespace Graphic_lib {

struct Axis : Graph_lib::Shape
{
    enum Orientation
    {
        x,
        y
    };

    Axis(Orientation d, Point xy, pix_numb length, pix_numb scale = 0,
         const std::string& label = "");

    void draw_lines () const override;

    void set_color (Graph_lib::Color c);
    // расстояние между насечками
    pix_numb dist;
    Graph_lib::Text label;
    Graph_lib::Lines notches;
};

}  // namespace Graphic_lib