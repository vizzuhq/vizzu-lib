#ifndef GEOM_QUADRILATERAL
#define GEOM_QUADRILATERAL

#include <array>

#include "base/geom/point.h"
#include "base/geom/rect.h"

namespace Geom
{

class ConvexQuad
{
public:
	typedef std::array<Point, 4u> Points;
	Points points;

	ConvexQuad() {}
	explicit ConvexQuad(const Points &points) : points(points) {}
	explicit ConvexQuad(const Geom::Rect &rect);
	static ConvexQuad Square(Point p0, Point p2);
	static ConvexQuad Isosceles(Geom::Point base0Middle, Geom::Point base1Middle, 
		double base0Length, double base1Legth);
	bool contains(const Point &p, double tolerance = 0.0) const;
	double area() const;
	Geom::Rect boundary() const;
};

}

#endif
