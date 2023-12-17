#include "frontend.h"

int main ()
{
    Frontend::Graphix_window grw(Graph_lib::Point(100, 100), 1300, 700, "FIDocalcus", 25);
    while (grw.window_is_open())
        grw.wait_for_button();
}