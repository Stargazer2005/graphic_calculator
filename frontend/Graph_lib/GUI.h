#pragma once

#include <stdexcept>

#include "Graph.h"
#include "Window.h"

namespace Graph_lib {

using Address = void*;                        // Address is a synonym for void*
using Callback = void (*)(Address, Address);  // FLTK's required function type for all callbacks

template <class W> W& reference_to (Address pw)
// treat an address as a reference to a W
{
    return *static_cast<W*>(pw);
}

class Widget
{
    // Widget is a handle to an Fl_widget - it is *not* an Fl_widget
    // We try to keep our interface classes at arm's length from FLTK

  public:
    Widget(Point _loc, pix_amount _width, pix_amount _height, const std::string& s, Callback cb)
        : loc{_loc}, width{_width}, height{_height}, label{s}, do_it{cb}
    {
    }

    // можем двигать и на отрицательные
    virtual void move (int dx, int dy)
    {
        if (dx > loc.x || dy > loc.y)
            throw std::invalid_argument("bad dx or dy");

        hide();
        pw->position(loc.x += dx, loc.y += dy);
        show();
    }

    virtual void hide () { pw->hide(); }

    virtual void show () { pw->show(); }

    virtual void attach (Window&) = 0;

    Window& window () { return *own; }

    virtual ~Widget() {}

    Widget& operator= (const Widget&) = delete;  // don't copy Widgets
    Widget(const Widget&) = delete;

  protected:
    Window* own;    // every Widget belongs to a Window
    Fl_Widget* pw;  // connection to the FLTK Widget

    Point loc;
    pix_amount width;
    pix_amount height;

    std::string label;
    Callback do_it;
};

struct Button : Widget
{
    Button(Point _loc, pix_amount _width, pix_amount _height, const std::string& label, Callback cb)
        : Widget{_loc, _width, _height, label, cb}
    {
    }

    void attach (Window&);
};

struct In_box : Widget
{
    In_box(Point _loc, pix_amount _width, pix_amount _height, const std::string& s)
        : Widget{_loc, _width, _height, s, nullptr}
    {
    }

    std::string get_string ();

    void attach (Window& win);
};

struct Out_box : Widget
{
    Out_box(Point _loc, pix_amount _width, pix_amount _height, const std::string& s)
        : Widget{_loc, _width, _height, s, nullptr}
    {
    }

    void put (int);
    void put (const std::string&);

    void attach (Window& win);
};

}  // namespace Graph_lib
