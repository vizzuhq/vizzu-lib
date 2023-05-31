#ifndef GEOM_QUADRILATERAL
#define GEOM_QUADRILATERAL

#include <array>

#include "base/geom/point.h"
#include "base/geom/rect.h"

namespace Geom
{

class ComplexQuadrilateral
{
public:
	typedef std::array<Point, 4u> Points;
	Points points;

	ComplexQuadrilateral() {}
	ComplexQuadrilateral(const Points &points) : points(points) {}
	static ComplexQuadrilateral Square(Point p0, Point p2);
	bool contains(const Point &p, double tolerance = 0.0) const;
	double area() const;
	Geom::Rect boundary() const;
};

}

#endif
