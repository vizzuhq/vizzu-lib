#include "triangle.h"

Geom::Triangle::Triangle(const Point &p0,
    const Point &p1,
    const Point &p2)
{
	points[0] = p0;
	points[1] = p1;
	points[2] = p2;
}

double Geom::Triangle::area() const
{
	auto A = points[2] - points[1];
	auto B = points[0] - points[1];
	return fabs(A ^ B) / 2;
}
