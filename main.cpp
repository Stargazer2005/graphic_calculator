#include "frontend.h"

int main ()
{
    Graphix_calc::Graphix_window grw(Graph_lib::Point(100, 100), 1000, 700, "FIDocalcus", 25);
    while (grw.window_is_open())
        grw.wait_for_button();
}