#include "Dext_box.h"

// std libs
#include <sstream>

// Graph_lib
using Graph_lib::Point;

namespace Graphix_calc {

Dext_box::Dext_box(Point xy, int w, int h, const std::string& s)
    : Graph_lib::Widget(xy, w, h, s, nullptr)
{
}

double Dext_box::get_int()
{
    Fl_Input& pi = Graph_lib::reference_to<Fl_Input>(pw);
    const char* p = pi.value();
    if (!isdigit(p[0]))
        return -999999;
    return atof(p);
}

void Dext_box::put_double(double i)
{
    Fl_Output& po = Graph_lib::reference_to<Fl_Output>(pw);
    std::stringstream ss;
    ss << i;
    po.value(ss.str().c_str());
}

void Dext_box::attach(Graph_lib::Window& win)
{
    pw = new Fl_Input{loc.x, loc.y, width, height, label.c_str()};
    own = &win;
}
}  // namespace Graphix_calc