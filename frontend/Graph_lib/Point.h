#pragma once

using pix_amount = unsigned int;

namespace Graph_lib {

struct Point
{
    pix_amount x, y;

    Point() : x{0}, y{0} {}

    constexpr Point(pix_amount _x, pix_amount _y) : x{_x}, y{_y} {}

    Point& operator+= (Point d)
    {
        x += d.x;
        y += d.y;
        return *this;
    }

    Point& operator-= (Point d)
    {
        x -= d.x;
        y -= d.y;
        return *this;
    }
};

inline Point operator+ (Point a, Point b) { return a += b; }

inline Point operator- (Point a, Point b) { return a -= b; }

inline bool operator== (Point a, Point b) { return a.x == b.x && a.y == b.y; }

inline bool operator!= (Point a, Point b) { return !(a == b); }

}  // namespace Graph_lib
