#include "analysis.h"
#include "graphic_window.h"
#include <Graph_Lib/Graph.h>
#include <Graph_lib/GUI.h>
#include <Graph_lib/Window.h>
#include <cmath>
#include <iostream>
using namespace Graph_lib;

int main ()
{
  TRACE_FUNC;
  // const std::string s = "1/x";
  // std::vector<std::string> rev_pol = reverse_polish(lexeme(s));
  // std::vector<Segment> v = get_info(rev_pol, -20.0, 20.0, 700 / 25, 40.0 / 400000.0);
  // std::cout << v.size();
  // for (size_t i = 0; i < v.size(); i++)
  // {
  //   std::cout << "{ " << v[i].start << ", " << v[i].stop << " }" << std::endl;
  // }
  Graphic_window grw(Point(100, 100), 1000, 700, "DESMOS", 25);
  while (grw.window_is_open())
  {
    grw.wait_for_button();
  }
}