// header
#include "Segmented_function.h"

// std libs
#include <cmath>

// servant
#include "../servant/constants.h"
using Front_consts::height_buff;
using Front_consts::max_scale;

namespace Graphix_calc {

Segmented_function::Segmented_function(const string& func, int scale, Point center, int max_x,
                                       int max_y)
    : func_str{function_string(func)}, segs{segments(max_x, max_y, scale)},
      seg_func{segmented_function(scale, center, max_x)},
      has_var{func.find('x') != std::string::npos}
{
}

vector<Segment> Segmented_function::segments(int max_x, int max_y, int scale) const
{
    // эксперементально вычесленная удачная точность для деления на сегменты
    double prec_seg = (((double)max_x / sqrt(scale))) / (abs(max_scale - scale));
    // переводим переданные пиксели в рациональные числа
    double l_border = -((double)max_x / (2 * scale));
    double r_border = ((double)max_x / (2 * scale));
    double h_border = (height_buff * (double)max_y / (2 * scale));

    // использование функции из бэкенда, которая дробит на рациональные числа
    vector<Segment> res = domain_segments(func_str, l_border, r_border, h_border, prec_seg);
    return res;
}

Vector_ref<Function> Segmented_function::segmented_function(int scale, Point center,
                                                            int max_x) const
{
    Vector_ref<Function> res;
    // это количество точек идеально подходит под наши задачи, однако если у нас нет переменной,
    // то это горизонтальная линия (по хорошему тут должна быть геометрическая прогрессия, но
    // ресурсов она требует неоправданно много)
    int point_amount = has_var ? (4 / 3) * max_x * sqrt(scale) : 2;
    for (auto seg : segs)
    {
        Function* f = new Function{func_str.calculate, seg.start,     seg.end,      center,
                                   point_amount,       double(scale), double(scale)};
        res.push_back(*f);
    }
    return res;
}

}  // namespace Graphix_calc
