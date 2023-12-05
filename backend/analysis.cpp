#include "analysis.h"
#include "back_func.h"

// Segment::Segment(double s, double v)
//     : s{start}, v{stop}

std::vector<Segment> get_info (std::vector<std::string>& rev_pol, double left_b, double right_b,
                               double height, double precision)
{
  // std::cout << "IN  ->"
  //           << "Get_info" << std::endl;
  std::vector<Segment> res;
  bool x_is_started = false;
  Segment seg;
  // std::cout << left_b << " " << right_b << " " << precision << std::endl;
  for (double x = left_b; x < right_b; x += precision)
  {
    if (!x_is_started)
    {
      try
      {
        double y = calc(rev_pol, x);
        if (((-height / 2) < y) && (y < height / 2))
        {
          seg.start = x;
          x_is_started = true;
        }
      }
      catch (const std::exception& e)
      {
      }
    }
    else
    {
      try
      {
        double y = calc(rev_pol, x);
        if ((y > height / 2) or (y < -height / 2))
        {
          seg.stop = x - precision;
          x_is_started = false;
          if (seg.start < seg.stop)
            res.push_back(seg);
        }
      }
      catch (const std::exception& e)
      {
        seg.stop = x - precision;
        x_is_started = false;
        if (seg.start < seg.stop)
          res.push_back(seg);
      }
    }
  }
  if (x_is_started)
  {
    seg.stop = right_b;
    if (seg.start < seg.stop)
      res.push_back(seg);
  }
  // std::cout << "OUT<- "
  //           << "get_info" << std::endl;
  return res;
}