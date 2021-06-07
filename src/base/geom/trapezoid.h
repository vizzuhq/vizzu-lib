#ifndef GEOM_TRAPEZOID
#define GEOM_TRAPEZOID

#include <array>

#include "base/math/interpolation.h"
#include "base/math/range.h"
#include "point.h"

namespace Geom
{

class Trapezoid
{
public:
	typedef std::array<Point, 4u> Points;
	Points points;

	Trapezoid(const Points &points) : points(points) {}
	static Trapezoid Square(Point p0, Point p2);
	bool contains(const Point &p, double tolerance = 0.0) const;
	double area() const;
};

class VerticalTrapezoid : public Trapezoid
{
public:
	using Trapezoid::Trapezoid;
	bool contains(const Point &p) const;
};

}

#endif
