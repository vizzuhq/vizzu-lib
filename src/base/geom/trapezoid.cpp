#include "trapezoid.h"

#include "base/math/tolerance.h"

#include "triangle.h"

using namespace Geom;

Trapezoid Trapezoid::Square(Point p0, Point p2)
{
	auto center = (p0 + p2) / 2;
	auto halfDiagonal = (p2 - p0) / 2;
	auto p1 = center + halfDiagonal.normal(false);
	auto p3 = center + halfDiagonal.normal(true);
	return Trapezoid({ p0, p1, p2, p3 });
}

bool Trapezoid::contains(const Point &p, double tolerance) const
{
	auto boundaryArea =
		  Triangle(std::array<Point, 3>{ points[0], points[1], p }).area()
		+ Triangle(std::array<Point, 3>{ points[1], points[2], p }).area()
		+ Triangle(std::array<Point, 3>{ points[2], points[3], p }).area()
		+ Triangle(std::array<Point, 3>{ points[3], points[0], p }).area();

	return Math::addTolerance(boundaryArea, tolerance) == area();
}

double Trapezoid::area() const
{
	return Triangle(std::array<Point, 3>{ points[0], points[1], points[2] }).area()
		 + Triangle(std::array<Point, 3>{ points[2], points[3], points[0] }).area();
}

bool VerticalTrapezoid::contains(const Point &p) const
{
	if (p.x < points[0].x || p.x > points[1].x) return false;

	auto xf =
	    Math::Range<double>(points[0].x, points[1].x).rescale(p.x);
	auto y0 = Math::interpolate(points[0].y, points[1].y, xf);
	auto y1 = Math::interpolate(points[3].y, points[2].y, xf);

	return Math::Range<double>(y0, y1).includes(p.y);
}
