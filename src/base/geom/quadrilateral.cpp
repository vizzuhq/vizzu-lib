#include "quadrilateral.h"

#include "base/math/tolerance.h"

#include "triangle.h"

using namespace Geom;

Rect ComplexQuadrilateral::boundary() const
{
	return Geom::Rect::Boundary(points);
}

ComplexQuadrilateral ComplexQuadrilateral::Square(Point p0, Point p2)
{
	auto center = (p0 + p2) / 2;
	auto halfDiagonal = (p2 - p0) / 2;
	auto p1 = center + halfDiagonal.normal(false);
	auto p3 = center + halfDiagonal.normal(true);
	return ComplexQuadrilateral({ p0, p1, p2, p3 });
}

bool ComplexQuadrilateral::contains(const Point &p, double tolerance) const
{
	auto boundaryArea =
		  Triangle(std::array<Point, 3>{ points[0], points[1], p }).area()
		+ Triangle(std::array<Point, 3>{ points[1], points[2], p }).area()
		+ Triangle(std::array<Point, 3>{ points[2], points[3], p }).area()
		+ Triangle(std::array<Point, 3>{ points[3], points[0], p }).area();

	return Math::addTolerance(boundaryArea, tolerance) == area();
}

double ComplexQuadrilateral::area() const
{
	return Triangle(std::array<Point, 3>{ points[0], points[1], points[2] }).area()
		 + Triangle(std::array<Point, 3>{ points[2], points[3], points[0] }).area();
}
