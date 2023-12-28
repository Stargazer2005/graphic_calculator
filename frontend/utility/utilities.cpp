#include "utilities.h"

// std libs
using std::string;
using std::to_string;

namespace Frontend_utilities {

string format (double num)
{
    string s = to_string(num);

    // remove after-dot zeroes
    size_t i = s.length() - 1;
    while (s[i] == '0')
    {
        s.erase(i);
        i--;
    }
    if (s[i] == '.')
        s.erase(i);

    // slice after-dot digits
    int prec = 4;
    i = s.find('.') + prec;
    while (i < s.length())
        s.erase(i);

    return s;
}

Graph_lib::Point convert_to_pix (Graph_lib::Point origin, Math_calc::Point p, double unit_intr)
{
    return {origin.x + pix_amount(p.x * unit_intr), origin.y + pix_amount(-p.y * unit_intr)};
}

Math_calc::Point convert_to_real (Graph_lib::Point origin, Graph_lib::Point p, double unit_intr)
{
    return {(double(p.x) - double(origin.x)) / unit_intr,
            (-double(p.y) + double(origin.y)) / unit_intr};
}

}  // namespace Frontend_utilities