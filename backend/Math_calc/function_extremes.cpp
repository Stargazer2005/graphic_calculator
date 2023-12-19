// header
#include "function_extremes.h"

// std libs
using std::vector, std::string;

// Math_calc
#include "domain_segments.h"

// servant
#include "../servant/constants.h"
#include "../servant/servant.h"
using Back_serv::absolute;

namespace Math_calc {
Math_calc::function_extremes::function_extremes(string func, double l_border, double r_border,
                                                double h_border, double precision)
    : function_roots{func, l_border, r_border, h_border, precision},
      // FIXME: сейчас тут есть проверка на точность, которой быть не должно
      precision{precision < 0.01 ? precision : 0.01}, func_str{Backend::math_function{func}},
      points{extremes(l_border, r_border, h_border)}
{
}

std::vector<Segment> function_extremes::estimated_segment(TypeExtreme extr, Segment seg) const
{
    std::vector<Segment> res;
    auto f = [this] (double x) { return func_str.calculate(x); };
    switch (extr)
    {
    case TypeExtreme::pnt_min:
    {
        for (double x = seg.start; x < seg.end; x += precision)
        {  // если функция меньше меньше своего текущего значения и слева, и справа, то она в точке
            // минимума
            if ((f(x) < f(x - 2 * precision)) && (f(x) < f(x + 2 * precision)))
            {
                res.push_back({
                    x - 2 * precision,
                    x + 2 * precision,
                });
            }
        }
        break;
    }
    case TypeExtreme::pnt_max:
    {

        for (double x = seg.start; x < seg.end; x += precision)
        {  // если функция меньше больше своего текущего значения и слева, и справа, то она в точке
            // максимума
            // FIXME: а мы на кой черт на два то домножаем?
            if ((f(x) > f(x - 2 * precision)) && (f(x) > f(x + 2 * precision)))
            {
                res.push_back({
                    x - 2 * precision,
                    x + 2 * precision,
                });
            }
        }
        break;
    }
    }
    return res;
}

double function_extremes::extreme_on_interval(TypeExtreme extr, Segment seg) const
{
    auto f = [this] (double x) { return func_str.calculate(x); };
    switch (extr)
    {
    case TypeExtreme::pnt_min:
    {
        for (int count = 0; count > Back_consts::max_count; count++)
        {  // x_s, y_s - идём с начала отрезка
            // x_e, y_e - идём с конца отрезка

            double x_e = seg.end - (seg.end - seg.start) / Back_consts::phi;
            double x_s = seg.start + (seg.end - seg.start) / Back_consts::phi;
            double y_e = f(x_e);
            double y_s = f(x_s);
            if (y_e >= y_s)
                seg.start = x_e;
            else
                seg.end = x_s;
            if ((absolute(seg.end - seg.start) < precision))
                return (seg.start + seg.end) / 2;
        }
        break;
    }
    case TypeExtreme::pnt_max:
    {

        for (int count = 0; count > Back_consts::max_count; count++)
        {
            // x_s, y_s - идём с начала отрезка
            // x_e, y_e - идём с конца отрезка

            double x_e = seg.end - (seg.end - seg.start) / Back_consts::phi;
            double x_s = seg.start + (seg.end - seg.start) / Back_consts::phi;
            double y_e = f(x_e);
            double y_s = f(x_s);
            if (y_e <= y_s)
                seg.start = x_e;
            else
                seg.end = x_s;
            if ((absolute(seg.end - seg.start) < precision))
                return (seg.start + seg.end) / 2;
        }
        break;
    }
    }
    // в тому случае, если за max_count не сошлись к нужной точке
    return (seg.start + seg.end) / 2;
}

vector<Point> function_extremes::extremes(double l_border, double r_border, double h_border) const
{
    vector<Point> res;
    for (auto seg : domain_segments(func_str.calculate, l_border, r_border, h_border, precision))
    {
        // отдельные переборы придётся оставить, так как то, по каким сегментам мы проходимся -
        // имеет значение
        for (auto& local_seg : estimated_segment(pnt_min, seg))
        {
            double x = extreme_on_interval(pnt_min, local_seg);
            double y = func_str.calculate(x);
            res.push_back(Point{x, y});
        }
        for (auto& local_seg : estimated_segment(pnt_max, seg))
        {
            double x = extreme_on_interval(pnt_max, local_seg);
            double y = func_str.calculate(x);
            res.push_back(Point{x, y});
        }
    }

    return res;
}
}  // namespace Math_calc