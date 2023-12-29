#pragma once

#include <string>
#include <vector>

#include "Point.h"
#include "fltk.h"

namespace Graph_lib {

class Shape;  // "forward declare" Shape
class Widget;

class Window : public Fl_Window
{
  public:
    using Fl_Window::resize;

    Window(pix_amount _width, pix_amount _height,
           const std::string& title);  // let the system pick the location
    Window(Point top_left_corner, pix_amount _width, pix_amount _height, const std::string& title);

    virtual ~Window() {}

    pix_amount w () const { return width; }

    pix_amount h () const { return height; }

    void resize (pix_amount _width, pix_amount _height)
    {
        width = _width;
        height = _height;
        size(_width, _height);
    }

    void set_label (const std::string& s) { label(s.c_str()); }

    void attach (Shape& s);
    void attach (Widget& w);

    void detach (Shape& s);   // remove s from shapes
    void detach (Widget& w);  // remove w from window (deactivate callbacks)

    void put_on_top (Shape& s);  // put p on top of other shapes

  protected:
    void draw ();

  private:
    std::vector<Shape*> shapes;  // shapes attached to window
    pix_amount width;
    pix_amount height;

    virtual void init ();
};

int gui_main ();  // invoke GUI library's main event loop

}  // namespace Graph_lib