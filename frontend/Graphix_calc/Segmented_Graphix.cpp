
#include "Segmented_Graphix.h"

// std libs
#include <cmath>
using std::string;
using std::vector;

// Graph_lib
using Graph_lib::Point;

// backend
using Math_func::function;
using Math_calc::domain_segments;
using Math_calc::Segment;

// servant
#include "../servant/constants.h"
using namespace Front_consts;

namespace Graphix_calc {

Segmented_Graphix::Segmented_Graphix(Math_func::function _func, double scale, Point center, int max_x,
                                     int max_y)
    : func{_func}, func_segs{segments(max_x, max_y, scale)},
      segs_der{derivative_segment(max_x, max_y, scale)},
      seged_graphix{segmented_graphix(scale, center, max_x)},
      seged_deriv{segmented_deriv(scale, center, max_x)}
{
}

vector<Segment> Segmented_Graphix::derivative_segment(int max_x, int max_y, double scale) const
{
    // эксперементально вычесленная удачная точность для деления на сегменты
    double prec_seg = (((double)max_x / sqrt(scale))) / (abs(max_scale - scale));
    // переводим переданные пиксели в веществ. числа
    double l_border = -((double)max_x / (2 * scale)) + ((double)func_box_w / (2 * scale));
    double r_border = ((double)max_x / (2 * scale));
    double h_border = (height_buff * (double)max_y / (2 * scale));

    // использование функции из бэкенда, которая дробит на веществ. числа
    vector<Segment> res =
        domain_segments(func.differentiate, l_border, r_border, h_border, prec_seg);
    return res;
}

vector<Segment> Segmented_Graphix::segments(int max_x, int max_y, double scale) const
{
    // эксперементально вычесленная удачная точность для деления на сегменты
    double prec_seg = (((double)max_x / sqrt(scale))) / (abs(max_scale - scale));
    // переводим переданные пиксели в веществ. числа
    double l_border = -((double)max_x / (2 * scale)) + ((double)func_box_w / (2 * scale));
    double r_border = ((double)max_x / (2 * scale));
    double h_border = (height_buff * (double)max_y / (2 * scale));

    // использование функции из бэкенда, которая дробит на веществ. числа
    vector<Segment> res = domain_segments(func.calculate, l_border, r_border, h_border, prec_seg);
    return res;
}

vector<Graphix*> Segmented_Graphix::segmented_graphix(double scale, Point center, int max_x) const
{
    vector<Graphix*> res;
    // это количество точек идеально подходит под наши задачи, однако если у нас нет переменной,
    // то это горизонтальная линия (по хорошему тут должна быть геометрическая прогрессия, но
    // ресурсов она требует неоправданно много)
    int point_amount = func.has_var() ? (4 / 3) * max_x * sqrt(scale) : 2;
    // int point_amount = has_var ? 4 * max_x : 2;
    for (auto seg : func_segs)
    {
        Graphix* f = new Graphix{func.calculate, seg.start, seg.end, center, point_amount, scale};
        res.push_back(f);
    }
    return res;
}

vector<Graphix*> Segmented_Graphix::segmented_deriv(double scale, Point center, int max_x) const
{
    vector<Graphix*> res;
    // это количество точек идеально подходит под наши задачи, однако если у нас нет переменной,
    // то это горизонтальная линия (по хорошему тут должна быть геометрическая прогрессия, но
    // ресурсов она требует неоправданно много)
    int point_amount = func.has_var() ? (4 / 3) * max_x * sqrt(scale) : 2;
    // int point_amount = has_var ? 4 * max_x : 2;

    for (auto seg : segs_der)
    {
        Graphix* f =
            new Graphix{func.differentiate, seg.start, seg.end, center, point_amount, scale};
        res.push_back(f);
    }
    return res;
}

}  // namespace Graphix_calc
