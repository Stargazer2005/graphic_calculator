#include <sstream>
#include <string>

#include "GUI.h"

using namespace Graph_lib;

void Button::attach(Window& win)
{
    pw = new Fl_Button{loc.x, loc.y, width, height, label.c_str()};
    pw->callback(reinterpret_cast<Fl_Callback*>(do_it), this);  // pass this widget
    own = &win;
}

int In_box::get_int()
{
    Fl_Input& pi = reference_to<Fl_Input>(pw);
    const char* p = pi.value();
    if (!isdigit(p[0]))
        return -999999;
    return atoi(p);
}

std::string In_box::get_string()
{
    Fl_Input& pi = reference_to<Fl_Input>(pw);
    return std::string(pi.value());
}

void In_box::attach(Window& win)
{
    pw = new Fl_Input{loc.x, loc.y, width, height, label.c_str()};
    own = &win;
}

void Out_box::put(int i)
{
    Fl_Output& po = reference_to<Fl_Output>(pw);
    std::stringstream ss;
    ss << i;
    po.value(ss.str().c_str());
}

void Out_box::put(const std::string& s) { reference_to<Fl_Output>(pw).value(s.c_str()); }

void Out_box::attach(Window& win)
{
    pw = new Fl_Output{loc.x, loc.y, width, height, label.c_str()};
    own = &win;
}
