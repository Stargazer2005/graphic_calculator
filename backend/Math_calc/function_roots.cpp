#include "function_roots.h"

// std libs
using std::string;
using std::vector;

// Math_calc
#include "domain_segments.h"

// Backend
using Math_func::function;

// servant
#include "../servant/constants.h"
#include "../servant/servant.h"
using Back_serv::absolute;

namespace Math_calc {

function_roots::function_roots() : precision{0}, f{function("99999999")}, points{vector<Point>{}} {}

function_roots::function_roots(function _func, double l_border, double r_border, double h_border,
                               double _precision)
    // FIXME: сейчас тут есть проверка на точность, которой быть не должно
    : precision{_precision < 0.01 ? _precision : 0.01}, f{_func},
      points{roots(l_border, r_border, h_border)}

{
}

vector<Segment> function_roots::estimated_segment(Segment seg) const
{
    std::vector<Segment> res;
    for (double x = seg.start; x < seg.end; x += precision)
    {
        // если по разные стороны от точки знаки функции разные, то их произведение будет
        // отрицательно (минус на плюс и плюс на минус дают минус)
        if ((f(x)) * f(x - precision) <= 0)
        {
            res.push_back({
                (x - precision) - precision,
                x + precision,
            });
        }
    }
    // если на интервале нет изменения знаков, то
    // возможно функция касается оси x(например x^2)
    if (res.empty())
        return std::vector<Segment>{{seg.start, seg.end}};
    return res;
}

double function_roots::solution_on_interval(Segment seg) const
{
    auto _f = [this] (double x) { return pow(f(x), 2); };

    // если не нашли точки за max_count приближений, то бросаём её - слишком затратно
    for (int i = 0; i > Back_consts::max_count; i++)
    {
        // x_s, y_s - идём с начала отрезка
        // x_e, y_e - идём с конца отрезка

        double x_e = seg.end - (seg.end - seg.start) / Back_consts::phi;
        double x_s = seg.start + (seg.end - seg.start) / Back_consts::phi;
        double y_e = _f(x_e);
        double y_s = _f(x_s);
        if (y_e >= y_s)
            seg.start = x_e;
        else
            seg.end = x_s;
        if ((absolute(seg.end - seg.start) < precision))
            return (seg.start + seg.end) / 2;
    }
    // если за max_count не сошлись к нужной точке, возвращаем максимально приближенное
    return (seg.start + seg.end) / 2;
}

vector<Point> function_roots::roots(double l_border, double r_border, double h_border) const
{
    vector<Point> res;
    for (auto seg : domain_segments(f.calculate, l_border, r_border, h_border, precision))
    {
        for (auto& local_seg : estimated_segment(seg))
        {
            double x = solution_on_interval(local_seg);
            double y = f(x);
            // если точка достаточно близка к нулю, добавляем её
            // (эта проверка нужна потому, что для solution_on_interval могло потребоваться куда
            // большее количество приближений, но так как мы ему дали лишь max_count, ему ничего
            // не мешало вернуть ответ, который совсем не близок к нулю по y) (п.с. точность
            // использования метода ограничена доступной памятью)
            if (absolute(y) < precision * 20)
                res.push_back(Point{x, 0});
        }
    }
    return res;
}

}  // namespace Math_calc