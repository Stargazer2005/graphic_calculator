#pragma once

// std libs
#include <string>
using std::string;

// Graphix_calc
#include "Graphix.h"
using Graphix_calc::Graphix;

// Backend
#include "../../backend.h"
using Backend::function_string;
using Math_calc::domain_segments;
using Math_calc::Segment;

// Graph_lib
#include <Graph_lib/Graph.h>
using Graph_lib::Point;
using Graph_lib::Vector_ref;

namespace Graphix_calc {

// функция, поделенная на сегменты, связанные с её областью определения
class Segmented_function
{
  public:
    Segmented_function(const string& func, int scale, Point center, int max_x, int max_y);

    // methods

    Vector_ref<Graphix> get_segmented_function () const { return seg_func; }

    Vector_ref<Graphix> get_segmented_derivative () const { return seg_der; }

  private:
    // variables

    // значение строки
    function_string func_str;
    bool has_var;
    // список отрезков, на котором рисуем функцию
    vector<Segment> segs_func;
    vector<Segment> segs_der;
    // отсегментированная функция
    Vector_ref<Graphix> seg_func;
    Vector_ref<Graphix> seg_der;

    // methods

    vector<Segment> derivative_segment (int max_x, int max_y, int scale) const;
    vector<Segment> segments (int max_x, int max_y, int scale) const;

    // функция, создающая ветктор Graphix - раздробленная функция на отрезки
    Vector_ref<Graphix> segmented_function (int scale, Point center, int max_x) const;
    Vector_ref<Graphix> segmented_derivative (int scale, Point center, int max_x) const;
};

}  // namespace Graphix_calc