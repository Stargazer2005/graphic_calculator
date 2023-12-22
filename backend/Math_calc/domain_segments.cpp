// header
#include "domain_segments.h"

// std libs
using std::function;
using std::vector;

namespace Math_calc {

vector<Segment> domain_segments (const function<double(double)>& calc, double min_x, double max_x,
                                 double max_y, double precision)
{
    // для большей точности
    max_x *= 2;
    double min_y = -max_y;
    std::vector<Segment> res;
    // начало отрезка уже было записано
    bool is_x_started = false;
    Segment seg;
    for (double x = min_x; x < max_x; x += precision)
    {
        if (!is_x_started)
        {
            // если мы еще не записали начало отрезка, проверяем на то, что значение не нарущает ОДЗ
            // и не больше максимально допустимой высоты
            try
            {
                double y = calc(x);
                if (((min_y / 2) < y) && (y < max_y / 2))
                {
                    seg.start = x;
                    is_x_started = true;
                }
            }
            catch (...)
            {  // в случае возникновение какой-либо ошибки, игнорируем эту точку, так как значение
               // на ней нам точно не подходит
            }
        }
        else
        {
            try
            {
                double y = calc(x);
                if ((y > max_y / 2) or (y < min_y / 2))
                {
                    seg.end = x - precision;
                    is_x_started = false;
                    if (seg.start < seg.end)
                        res.push_back(seg);
                }
            }
            catch (...)
            {
                // в этом случае обрезаем отрезок предыдущим значением, так как на текущем мы
                // получили ошибку
                seg.end = x - precision;
                is_x_started = false;
                if (seg.start < seg.end)
                    res.push_back(seg);
            }
        }
    }
    // если в самом конце не успели закрыть отрезок, делаем это правой границей (скорее всего, мы её
    // просто перескочили или дошли до max_x - precision)
    if (is_x_started)
    {
        seg.end = max_x;
        if (seg.start < seg.end)
            res.push_back(seg);
    }
    return res;
}

}  // namespace Math_calc