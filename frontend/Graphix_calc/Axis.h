#pragma once

// std libs
#include <string>

// Graph_lib
#include "../Graph_lib/Graph.h"

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

    Axis(Orientation orient, Graph_lib::Point origin, pix_amount length, double _unit_intr,
         const std::string& label_text);

    Axis(Orientation orient, Graph_lib::Point origin, pix_amount length, double _unit_intr,
         pix_amount _mark_intr, const std::string& label_text);

    // methods

    void draw_lines () const override;

    void set_color (Graph_lib::Color c);

    void init (Orientation orient, Graph_lib::Point origin, pix_amount length);

  private:
    // vars

    // расстояние между насечками
    double unit_intr;
    // расстояние между отмечаемыми насечками
    pix_amount mark_intr;
    Graph_lib::Text label;
    std::vector<Graph_lib::Text*> marks;
    Graph_lib::Lines notches;
};

}  // namespace Graphix_calc