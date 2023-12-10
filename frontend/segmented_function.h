#pragma once

#include "../backend/function_string.h"
#include <Graph_lib/Graph.h>
#include <string>

using Graph_lib::Point;
using std::string;

namespace Graphix_calc {

struct Segment
{
    double start;
    double stop;
};

class Segmented_function
{
  private:
    // variables

    // значение строки
    Backend::function_string expr;
    // список отрезков, на котором рисуем функцию
    vector<Segment> segs;
    // отсегментированная функция
    Graph_lib::Vector_ref<Graph_lib::Function> seg_func;
    bool has_var;

    // metods

    // вспомогательная функция, которая дробит на отрезки (в рациональных числах)
    vector<Segment> domain_segments (double l_border, double r_border, double height,
                                     double precision) const;
    // функция, дробящая на сегменты в пикселях
    vector<Segment> segments (int max_x, int max_y, int scale);

    // функция, создающая ветктор Function - раздробленная функция на отрезки
    Graph_lib::Vector_ref<Graph_lib::Function>
    segmented_function (int scale, Graph_lib::Point center, int max_x);

  public:
    Segmented_function(string str, int scale, Point center, int max_x, int max_y);

    Graph_lib::Vector_ref<Graph_lib::Function> get_segmented_function ();
};

}  // namespace Graphix_calc