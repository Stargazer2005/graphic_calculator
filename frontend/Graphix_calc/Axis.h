#pragma once

// std libs
#include <string>
using std::string;

// Graph_lib
#include <Graph_lib/Graph.h>
using Graph_lib::Color, Graph_lib::Point;

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

    Axis(Orientation d, Point center, int length, int scale, const string& label);

    // methods

    void draw_lines () const override;

    void set_color (Color c);

  private:
    // variables

    //  расстояние между насечками
    int dist;
    Graph_lib::Text label;
    Graph_lib::Lines notches;
};

}  // namespace Graphix_calc