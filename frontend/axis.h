#pragma once

#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

using Graph_lib::Point;

using pix_numb = int;

namespace Graphix_calc {

// наш собственный класс осей
class Axis : public Graph_lib::Shape
{
  public:
    enum Orientation
    {
        x,
        y
    };

    Axis(Orientation d, Point xy, pix_numb length, pix_numb scale = 0,
         const std::string& label = "");

    void draw_lines () const override;

    void set_color (Graph_lib::Color c);

  private:
    // расстояние между насечками
    pix_numb dist;
    Graph_lib::Text label;
    Graph_lib::Lines notches;
};

}  // namespace Graphix_calc