#include <algorithm>
#include <fstream>
#include <map>
#include <random>
#include <stdexcept>

#include "Graph.h"

namespace Graph_lib {

Shape::Shape(std::initializer_list<Point> lst)
{
    for (Point p : lst)
        add(p);
}

void Shape::draw_lines() const
{
    if (color().visibility() && 1 < points.size())  // draw sole pixel?
        for (unsigned int i = 1; i < points.size(); ++i)
            fl_line(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
}

void Shape::draw() const
{
    Fl_Color oldc = fl_color();  // there is no good portable way of retrieving the current style
    fl_color(lcolor.as_int());
    fl_line_style(ls.style(), ls.width());
    draw_lines();
    fl_color(oldc);  // reset color (to pevious) and style (to default)
    fl_line_style(0);
}

inline std::pair<double, double> line_intersect (Point p1, Point p2, Point p3, Point p4,
                                                 bool& parallel)
// does two lines (p1,p2) and (p3,p4) intersect?
// if yes return the distance of the intersect point as distances from p1
{
    double x1 = p1.x;
    double x2 = p2.x;
    double x3 = p3.x;
    double x4 = p4.x;
    double y1 = p1.y;
    double y2 = p2.y;
    double y3 = p3.y;
    double y4 = p4.y;

    double denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    if (denom == 0)
    {
        parallel = true;
        return std::make_pair(0., 0.);
    }
    parallel = false;
    return std::make_pair(((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom,
                          ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom);
}

bool line_segment_intersect (Point p1, Point p2, Point p3, Point p4, Point& intersection)
// intersection between two line segments
// Returns true if the two segments intersect,
// in which case intersection is set to the point of intersection
{
    bool parallel;
    std::pair<double, double> u = line_intersect(p1, p2, p3, p4, parallel);

    if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1)
        return false;

    intersection.x = p1.x + u.first * (p2.x - p1.x);
    intersection.y = p1.y + u.first * (p2.y - p1.y);
    return true;
}

void Polygon::add(Point p)
{
    int np = number_of_points();

    // check that the new line isn't parallel to the previous one
    if (1 < np)
    {
        if (p == point(np - 1))
            throw std::invalid_argument("polygon point equal to previous point");

        bool parallel;
        line_intersect(point(np - 1), p, point(np - 2), point(np - 1), parallel);

        if (parallel)
            throw std::invalid_argument("two polygon points lie in a straight line");
    }

    // check that new segment doesn't interset and old point
    for (int i = 1; i < np - 1; ++i)
    {
        Point ignore{0, 0};
        if (line_segment_intersect(point(np - 1), p, point(i - 1), point(i), ignore))
            throw std::invalid_argument("intersect in polygon");
    }

    Closed_polyline::add(p);
}

void Polygon::draw_lines() const
{
    if (number_of_points() < 3)
        throw std::invalid_argument("less than 3 points in a Polygon");
    Closed_polyline::draw_lines();
}

void Open_polyline::draw_lines() const
{
    if (fill_color().visibility())
    {
        fl_color(fill_color().as_int());
        fl_begin_complex_polygon();
        for (int i = 0; i < number_of_points(); ++i)
        {
            fl_vertex(point(i).x, point(i).y);
        }
        fl_end_complex_polygon();
        fl_color(color().as_int());  // reset color
    }

    if (color().visibility())
        Shape::draw_lines();
}

void Closed_polyline::draw_lines() const
{
    Open_polyline::draw_lines();

    if (color().visibility())  // draw closing line
        fl_line(point(number_of_points() - 1).x, point(number_of_points() - 1).y, point(0).x,
                point(0).y);
}

void Shape::move(int dx, int dy)
{
    for (unsigned int i = 0; i < points.size(); ++i)
    {
        points[i].x += dx;
        points[i].y += dy;
    }
}

Shape::~Shape()
{ /* all done */
}

void Lines::draw_lines() const
{
    if (color().visibility())
        for (int i = 1; i < number_of_points(); i += 2)
            fl_line(point(i - 1).x, point(i - 1).y, point(i).x, point(i).y);
}

void Text::draw_lines() const
{
    int ofnt = fl_font();
    int osz = fl_size();
    fl_font(fnt.as_int(), fnt_sz);
    fl_draw(lab.c_str(), point(0).x, point(0).y);
    fl_font(ofnt, osz);
}

void Rectangle::draw_lines() const
{
    if (fill_color().visibility())  // fill
    {
        fl_color(fill_color().as_int());
        fl_rectf(point(0).x, point(0).y, w, h);
        fl_color(color().as_int());  // reset color
    }

    if (color().visibility())  // edge on top of fill
        fl_rect(point(0).x, point(0).y, w, h);
}

void Circle::draw_lines() const
{
    if (fill_color().visibility())  // fill
    {
        fl_color(fill_color().as_int());
        fl_pie(point(0).x, point(0).y, r + r - 1, r + r - 1, 0, 360);
        fl_color(color().as_int());  // reset color
    }

    if (color().visibility())
        fl_arc(point(0).x, point(0).y, r + r, r + r, 0, 360);
}

void Ellipse::draw_lines() const
{
    if (fill_color().visibility())  // fill
    {
        fl_color(fill_color().as_int());
        fl_pie(point(0).x, point(0).y, w + w - 1, h + h - 1, 0, 360);
        fl_color(color().as_int());  // reset color
    }

    if (color().visibility())
        fl_arc(point(0).x, point(0).y, w + w, h + h, 0, 360);
}

Point Ellipse::focus1() const
{
    double d = std::round(std::sqrt(double(w * w - h * h)));
    return {center().x + int(d), center().y};
}

Point Ellipse::focus2() const
{
    double d = std::round(std::sqrt(double(w * w - h * h)));
    return {center().x - int(d), center().y};
}

void Ellipse::set_major(int ww)
{
    // maintain the center
    set_point(0, Point{center().x - ww, center().y - h});
    w = ww;
}

void Ellipse::set_minor(int hh)
{
    // maintain the center
    set_point(0, Point{center().x - w, center().y - hh});
    h = hh;
}

void draw_mark (Point xy, char c)
{
    static const int dx = 4;
    static const int dy = 4;
    std::string m{c};
    fl_draw(m.c_str(), xy.x - dx, xy.y + dy);
}

Marked_polyline::Marked_polyline(const std::string& m, std::initializer_list<Point> lst)
    : Open_polyline{lst}, mark{m}
{
    if (m == "")
        mark = "*";
}

void Marked_polyline::draw_lines() const
{
    Open_polyline::draw_lines();
    for (int i = 0; i < number_of_points(); ++i)
        draw_mark(point(i), mark[i % mark.size()]);
}

std::map<std::string, Suffix::Encoding> suffix_map;

int init_suffix_map ()
{
    suffix_map["png"] = Suffix::png;
    suffix_map["PNG"] = Suffix::png;
    suffix_map["jpg"] = Suffix::jpg;
    suffix_map["JPG"] = Suffix::jpg;
    suffix_map["jpeg"] = Suffix::jpg;
    suffix_map["JPEG"] = Suffix::jpg;
    suffix_map["gif"] = Suffix::gif;
    suffix_map["GIF"] = Suffix::gif;
    suffix_map["bmp"] = Suffix::bmp;
    suffix_map["BMP"] = Suffix::bmp;
    return 0;
}

Suffix::Encoding get_encoding (const std::string& s)
// try to deduce type from file name using a lookup table
{
    [[maybe_unused]] static int x = init_suffix_map();

    std::string::const_iterator p = std::find(s.begin(), s.end(), '.');
    if (p == s.end())
        return Suffix::none;  // no suffix

    std::string suf{p + 1, s.end()};
    return suffix_map[suf];
}

bool can_open (const std::string& s)
// check if a file named s exists and can be opened for reading
{
    return static_cast<bool>(std::ifstream{s});
}

Image::Image(Point xy, const std::string& s, Suffix::Encoding e) : w{0}, h{0}, fn{xy, ""}
// somewhat overelaborate constructor
// because errors related to image files can be such a pain to debug
{
    add(xy);

    if (!can_open(s))
    {
        fn.set_label("cannot open \"" + s + '\"');
        p = new Bad_image{30, 20};  // the "error image"
        return;
    }

    if (e == Suffix::none)
        e = get_encoding(s);

    switch (e)
    {
    case Suffix::png:
        p = new Fl_PNG_Image{s.c_str()};
        break;
    case Suffix::jpg:
        p = new Fl_JPEG_Image{s.c_str()};
        break;
    case Suffix::gif:
        p = new Fl_GIF_Image{s.c_str()};
        break;
    default:  // Unsupported image encoding
        fn.set_label("unsupported file type \"" + s + '\"');
        p = new Bad_image{30, 20};  // the "error image"
    }
}

void Image::draw_lines() const
{
    if (fn.label() != "")
        fn.draw_lines();

    if (w && h)
        p->draw(point(0).x, point(0).y, w, h, cx, cy);
    else
        p->draw(point(0).x, point(0).y);
}

}  // namespace Graph_lib
