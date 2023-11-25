#ifndef GRAPH_WIN_H
#define GRAPH_WIN_H

#include <Graph_Lib/Graph.h>
#include <Graph_lib/GUI.h>
#include <Graph_lib/Simple_window.h>
#include <string>
using namespace Graph_lib;

class Graphic_window : public Simple_window
{
  public:
    Graphic_window(Point xy, int w, int h, const std::string& title, int scale);

  private:
    int s;
    Axis x_, y_;
};
#endif  // GRAPH_WIN_H