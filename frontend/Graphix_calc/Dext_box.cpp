#include "Dext_box.h"

// std libs
#include <sstream>
using std::string;

// Graph_lib
using Graph_lib::Point;
using Graph_lib::reference_to;

namespace Graphix_calc {

Dext_box::Dext_box(Point xy, pix_amount _width, pix_amount _height, const std::string& s)
    : Graph_lib::Widget(xy, _width, _height, s, nullptr)
{
}

std::string Dext_box::get_string()
{
    Fl_Input& pi = reference_to<Fl_Input>(pw);
    return std::string(pi.value());
}

void Dext_box::put_string(const std::string& s) { reference_to<Fl_Output>(pw).value(s.c_str()); }

void Dext_box::attach(Graph_lib::Window& win)
{
    pw = new Fl_Input{int(loc.x), int(loc.y), int(width), int(height), label.c_str()};
    own = &win;
}
}  // namespace Graphix_calc