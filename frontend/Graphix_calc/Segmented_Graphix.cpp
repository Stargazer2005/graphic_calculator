
#include "Segmented_Graphix.h"

// std libs
#include <cmath>
using std::string;
using std::vector;

// Graph_lib
using Graph_lib::Point;

// backend
using Math_calc::domain_segments;
using Math_calc::Segment;
using Math_func::function;

// utility
#include "../utility/constants.h"
using namespace Frontend_consts;
#include "../utility/utilities.h"
using namespace Frontend_utilities;

namespace Graphix_calc {

Segmented_Graphix::Segmented_Graphix(std::function<double(double)> _calc, double unit_intr,
                                     Point origin, Graph_lib::Point left_bottom,
                                     Graph_lib::Point right_top)
    : calc{_calc}, func_segs{segments(unit_intr, origin, left_bottom, right_top)},
      seged_graphix{segmented_graphix(unit_intr, origin, left_bottom, right_top)}
{
}

void Segmented_Graphix::draw_lines() const
{
    Shape::draw_lines();
    for (const auto& garphix_part : seged_graphix)
        garphix_part->draw();
}

void Segmented_Graphix::set_color(Graph_lib::Color c)
{
    Shape::set_color(c);
    for (const auto& garphix_part : seged_graphix)
        garphix_part->set_color(c);
}

vector<Segment> Segmented_Graphix::segments(double unit_intr, Graph_lib::Point origin,
                                            Graph_lib::Point left_bottom,
                                            Graph_lib::Point right_top) const
{
    // эксперементально вычесленная удачная точность для деления на сегменты
    // (говно неработающее)
    // double prec_seg = ((double(r_border) / sqrt(unit_intr))) / (abs(max_unit_intr - unit_intr));

    // FIXME: естественно, она не должна быть константой
    double prec_seg = 0.0001;

    // использование функции из бэкенда, которая дробит на веществ. числа
    vector<Segment> res = domain_segments(calc, convert_to_real(origin, left_bottom, unit_intr),
                                          convert_to_real(origin, right_top, unit_intr), prec_seg);
    return res;
}

vector<Graphix*> Segmented_Graphix::segmented_graphix(double unit_intr, Graph_lib::Point origin,
                                                      Graph_lib::Point left_bottom,
                                                      Graph_lib::Point right_top) const
{
    vector<Graphix*> res;
    // это количество точек идеально подходит под наши задачи, однако если у нас нет переменной,
    // то это горизонтальная линия (по хорошему тут должна быть геометрическая прогрессия, но
    // ресурсов она требует неоправданно много)
    int point_amount = (4 / 3) * right_top.x * sqrt(unit_intr);
    for (const auto& seg : func_segs)
    {
        Graphix* f = new Graphix{calc, seg.start, seg.end, origin, point_amount, unit_intr};
        res.push_back(f);
    }
    return res;
}

}  // namespace Graphix_calc
