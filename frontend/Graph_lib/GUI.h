#pragma once

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
    Widget(Point xy, int w, int h, const std::string& s, Callback cb)
        : loc{xy}, width{w}, height{h}, label{s}, do_it{cb}
    {
    }

    virtual void move (int dx, int dy)
    {
        hide();
        pw->position(loc.x += dx, loc.y += dy);
        show();
    }

    virtual void hide () { pw->hide(); }

    virtual void show () { pw->show(); }

    virtual void attach (Window&) = 0;

    Window& window () { return *own; }

    Point loc;
    int width;
    int height;
    std::string label;
    Callback do_it;

    virtual ~Widget() {}

    Widget& operator= (const Widget&) = delete;  // don't copy Widgets
    Widget(const Widget&) = delete;

  protected:
    Window* own;    // every Widget belongs to a Window
    Fl_Widget* pw;  // connection to the FLTK Widget
};

struct Button : Widget
{
    Button(Point xy, int w, int h, const std::string& label, Callback cb)
        : Widget{xy, w, h, label, cb}
    {
    }

    void attach (Window&);
};

struct In_box : Widget
{
    In_box(Point xy, int w, int h, const std::string& s) : Widget{xy, w, h, s, nullptr} {}

    int get_int ();
    std::string get_string ();

    void attach (Window& win);
};

struct Out_box : Widget
{
    Out_box(Point xy, int w, int h, const std::string& s) : Widget{xy, w, h, s, nullptr} {}

    void put (int);
    void put (const std::string&);

    void attach (Window& win);
};

}  // namespace Graph_lib
