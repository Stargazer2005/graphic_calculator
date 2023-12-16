#pragma once

// std libs
#include <string>
using std::string;

// Backend
#include "../../backend.h"
using Backend::function_string;
using Math_calc::domain_segments;
using Math_calc::Segment;

// Graph_lib
#include <Graph_lib/Graph.h>
using Graph_lib::Function;
using Graph_lib::Point;
using Graph_lib::Vector_ref;

namespace Graphix_calc {

// функция, поделенная на сегменты, связанные с её областью определения
class Segmented_function
{
  public:
    Segmented_function(const string& str, int scale, Point center, int max_x, int max_y);

    // methods

    Vector_ref<Function> get_segmented_function () const { return seg_func; }

  private:
    // variables

    // значение строки
    function_string func_str;
    bool has_var;
    // список отрезков, на котором рисуем функцию
    vector<Segment> segs;
    // отсегментированная функция
    Vector_ref<Function> seg_func;

    // methods

    // функция, дробящая на сегменты в пикселях
    vector<Segment> segments (int max_x, int max_y, int scale) const;

    // функция, создающая ветктор Function - раздробленная функция на отрезки
    Vector_ref<Function> segmented_function (int scale, Point center, int max_x) const;
};

}  // namespace Graphix_calc