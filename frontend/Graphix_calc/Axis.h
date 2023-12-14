#pragma once

// std libs
#include <string>
using std::string;

// Graph_lib
#include <Graph_lib/Graph.h>
using Graph_lib::Color;
using Graph_lib::Point;

using pix_numb = int;

namespace Graphix_calc {

// наш собственный класс осей
class Axis : public Graph_lib::Shape
{
  public:
    enum Orientation
    {
        horisontal,
        vertical
    };

    Axis(Orientation d, Point center, pix_numb length, pix_numb scale, const string& label);

    // methods

    void draw_lines () const override;

    void set_color (Color c);

  private:
    // variables

    //  расстояние между насечками
    pix_numb dist;
    Graph_lib::Text label;
    Graph_lib::Lines notches;
};

}  // namespace Graphix_calc