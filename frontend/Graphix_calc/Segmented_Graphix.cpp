
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

namespace Graphix_calc {

Segmented_Graphix::Segmented_Graphix(Math_func::function _func, double scale, Point center,
                                     int r_border, int h_border)
    : func{_func}, func_segs{segments(-r_border, r_border, h_border, scale)},
      segs_der{derivative_segment(-r_border, r_border, h_border, scale)},
      seged_graphix{segmented_graphix(scale, center, r_border)},
      seged_deriv{segmented_deriv(scale, center, r_border)}
{
}

vector<Segment> Segmented_Graphix::derivative_segment(int l_border, int r_border, int h_border,
                                                      double scale) const
{
    // эксперементально вычесленная удачная точность для деления на сегменты
    double prec_seg = ((double(r_border) / sqrt(scale))) / (abs(max_scale - scale));
    // переводим переданные пиксели в веществ. числа
    double min_x = -((double)r_border / (2 * scale)) + ((double)func_box_w / (2 * scale));
    double max_x = ((double)r_border / (2 * scale));
    double max_y = (height_buff * (double)h_border / (2 * scale));

    // использование функции из бэкенда, которая дробит на веществ. числа
    vector<Segment> res = domain_segments(func.differentiate, min_x, max_x, max_y, prec_seg);
    return res;
}

vector<Segment> Segmented_Graphix::segments(int l_border, int r_border, int h_border,
                                            double scale) const
{
    // эксперементально вычесленная удачная точность для деления на сегменты
    double prec_seg = ((double(r_border) / sqrt(scale))) / (abs(max_scale - scale));
    // переводим переданные пиксели в веществ. числа
    double min_x = -((double)r_border / (2 * scale)) + ((double)func_box_w / (2 * scale));
    double max_x = ((double)r_border / (2 * scale));
    double max_y = (height_buff * (double)h_border / (2 * scale));

    // использование функции из бэкенда, которая дробит на веществ. числа
    vector<Segment> res = domain_segments(func.calculate, min_x, max_x, max_y, prec_seg);
    return res;
}

vector<Graphix*> Segmented_Graphix::segmented_graphix(double scale, Point center,
                                                      int r_border) const
{
    vector<Graphix*> res;
    // это количество точек идеально подходит под наши задачи, однако если у нас нет переменной,
    // то это горизонтальная линия (по хорошему тут должна быть геометрическая прогрессия, но
    // ресурсов она требует неоправданно много)
    int point_amount = func.has_var() ? (4 / 3) * r_border * sqrt(scale) : 2;
    for (const auto& seg : func_segs)
    {
        Graphix* f = new Graphix{func.calculate, seg.start, seg.end, center, point_amount, scale};
        res.push_back(f);
    }
    return res;
}

vector<Graphix*> Segmented_Graphix::segmented_deriv(double scale, Point center, int r_border) const
{
    vector<Graphix*> res;
    // это количество точек идеально подходит под наши задачи, однако если у нас нет переменной,
    // то это горизонтальная линия (по хорошему тут должна быть геометрическая прогрессия, но
    // ресурсов она требует неоправданно много)
    int point_amount = func.has_var() ? (4 / 3) * r_border * sqrt(scale) : 2;
    for (const auto& seg : segs_der)
    {
        Graphix* f =
            new Graphix{func.differentiate, seg.start, seg.end, center, point_amount, scale};
        res.push_back(f);
    }
    return res;
}

}  // namespace Graphix_calc
