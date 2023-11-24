#include "graphic_window.h"
#include <Graph_Lib/Graph.h>
#include <Graph_lib/GUI.h>
#include <Graph_lib/Window.h>
#include <iostream>

using namespace Graph_lib;

int main ()
{
  Graphic_window grw(Point(100, 100), 1000, 700, "DESMOS", 50);
  std::cout << grw.get_func() << '\n';
  while (grw.window_is_open())
  {
    grw.wait_for_button();
    if (grw.get_func() != "")
    {
      std::cout << grw.get_func() << '\n';
    }
  }
}