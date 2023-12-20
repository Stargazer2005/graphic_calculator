#include "frontend.h"

int main ()
{
    Frontend::Graphix_window grw({100, 100}, 1000, 700, "FIDocalcus", 50);
    while (grw.window_is_open())
        grw.wait_for_button();
}