#pragma once

namespace Math_calc {

// вещественная точка
struct Point
{
    Point(double x, double y) : x{x}, y{y} {}

    Point() : x{0}, y{0} {}

    double x, y;
};

// вещественный отрезок
struct Segment
{
    Segment(double start, double stop) : start{start}, end{stop} {}

    Segment() : start{0}, end{0} {}

    double start, end;
};

}  // namespace Math_calc
