#include "quadrilateral.h"

#include "base/math/tolerance.h"

#include "triangle.h"

using namespace Geom;

ConvexQuad::ConvexQuad(const Geom::Rect &rect)
{
	points[0] = rect.pos;
	points[1] = rect.pos + Point(rect.size.x, 0.0);
	points[2] = rect.pos + rect.size;
	points[3] = rect.pos + Point(0.0, rect.size.y);
}

Rect ConvexQuad::boundary() const
{
	return Geom::Rect::Boundary(points);
}

ConvexQuad ConvexQuad::Square(Point p0, Point p2)
{
	auto center = (p0 + p2) / 2;
	auto halfDiagonal = (p2 - p0) / 2;
	auto p1 = center + halfDiagonal.normal(false);
	auto p3 = center + halfDiagonal.normal(true);
	return ConvexQuad({p0, p1, p2, p3});
}

ConvexQuad ConvexQuad::Isosceles(
	Geom::Point base0Middle, Geom::Point base1Middle, 
	double base0Length, double base1Length)
{
	auto dir = (base1Middle - base0Middle).normalized();

	return ConvexQuad({
	    base0Middle + dir.normal(false) * (base0Length / 2),
	    base0Middle + dir.normal(true) * (base0Length / 2),
	    base1Middle + dir.normal(true) * (base1Length / 2),
	    base1Middle + dir.normal(false) * (base1Length / 2)
	});
}


bool ConvexQuad::contains(const Point &p, double tolerance) const
{
	auto boundaryArea =
	    Triangle(std::array<Point, 3>{points[0], points[1], p}).area()
	    + Triangle(std::array<Point, 3>{points[1], points[2], p})
	          .area()
	    + Triangle(std::array<Point, 3>{points[2], points[3], p})
	          .area()
	    + Triangle(std::array<Point, 3>{points[3], points[0], p})
	          .area();

	return Math::AddTolerance(boundaryArea, tolerance) <= area();
}

double ConvexQuad::area() const
{
	return Triangle(
	           std::array<Point, 3>{points[0], points[1], points[2]})
	           .area()
	     + Triangle(
	         std::array<Point, 3>{points[2], points[3], points[0]})
	           .area();
}
