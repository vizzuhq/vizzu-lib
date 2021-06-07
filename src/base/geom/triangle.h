#ifndef GEOM_TRIANGLE
#define GEOM_TRIANGLE

#include <array>

#include "point.h"

namespace Geom
{

class Triangle
{
public:
	std::array<Point, 3> points;

	Triangle(const std::array<Point, 3> &points) : points(points) {}
	Triangle(const Point &p0, const Point &p1, const Point &p2);

	double area() const;
};

}

#endif
