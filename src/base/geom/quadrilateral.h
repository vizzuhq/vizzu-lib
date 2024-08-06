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
	using Points = std::array<Point, 4U>;
	Points points;

	ConvexQuad() = default;
	explicit ConvexQuad(const Points &points) : points(points) {}
	[[nodiscard]] static ConvexQuad Isosceles(Point base0Middle,
	    Point base1Middle,
	    double base0Length,
	    double base1Length);

	[[nodiscard]] double distance(const Point &point) const;
};

}

#endif
