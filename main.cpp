#include "frontend/frontend.h"

int main ()
{
    Graphic_lib::Graphic_window grw(Graph_lib::Point(100, 100), 1000, 700, "FIDocalcus", 25);
    while (grw.window_is_open())
        grw.wait_for_button();
}