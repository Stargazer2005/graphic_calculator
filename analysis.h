#pragma once
#include <string>
#include <vector>

struct Segment
{
  // Segment(double s, double v);
  double start;
  double stop;
};

std::vector<Segment> get_info (std::vector<std::string>& rev_pol, double left_b, double right_b,
                               double height, double precision);