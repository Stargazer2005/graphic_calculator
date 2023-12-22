#pragma once

// std libs
#include <string>

// Graph_lib
#include <Graph_lib/Graph.h>

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

    Axis(Orientation d, Graph_lib::Point center, int length, double scale,
         const std::string& label);

    // methods

    void draw_lines () const override;

    void set_color (Graph_lib::Color c);

  private:
    // variables

    //  расстояние между насечками
    int dist;
    Graph_lib::Text label;
    std::vector<Graph_lib::Text*> marks;
    Graph_lib::Lines notches;
};

}  // namespace Graphix_calc