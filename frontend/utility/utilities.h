// std libs
#include <string>

// Graph_lib
#include "../Graph_lib/Point.h"

// Backend
#include "../../backend.h"

namespace Frontend_utilities {

std::string format (double num);

Graph_lib::Point convert_to_pix (Graph_lib::Point origin, Math_calc::Point p, double unit_intr);

Math_calc::Point convert_to_real (Graph_lib::Point origin, Graph_lib::Point p, double unit_intr);

}  // namespace Frontend_utilities