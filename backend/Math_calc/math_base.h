#pragma once

namespace Math_calc {

// MEANS: вещественная точка
struct Point
{
    inline Point(double _x, double _y) : x{_x}, y{_y} {}

    inline Point() : x{0}, y{0} {}

    // vars

    double x, y;

    // ~vars
};

// MEANS: вещественный отрезок
struct Segment
{
    inline Segment(double _start, double _end) : start{_start}, end{_end} {}

    inline Segment() : start{-0}, end{0} {}

    // vars

    double start, end;

    // ~vars
};

}  // namespace Math_calc
