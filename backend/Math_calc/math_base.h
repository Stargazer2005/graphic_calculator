#pragma once

namespace Math_calc {

// MEMO: вещественная точка
struct Point
{
    Point(double _x, double _y) : x{_x}, y{_y} {}

    Point() : x{0}, y{0} {}

    // vars

    double x, y;
};

// MEMO: вещественный отрезок
struct Segment
{
    Segment(double _start, double _end) : start{_start}, end{_end} {}

    Segment() : start{-0}, end{0} {}

    // vars

    double start, end;
};

}  // namespace Math_calc
