#include "quadrilateral.h"

#include "base/math/tolerance.h"

#include "triangle.h"

namespace Geom
{

ConvexQuad ConvexQuad::Isosceles(Point base0Middle,
    Point base1Middle,
    double base0Length,
    double base1Length)
{
	auto dir = (base1Middle - base0Middle).normalized();

	return ConvexQuad(
	    {base0Middle + dir.normal(false) * (base0Length / 2),
	        base0Middle + dir.normal(true) * (base0Length / 2),
	        base1Middle + dir.normal(true) * (base1Length / 2),
	        base1Middle + dir.normal(false) * (base1Length / 2)});
}

double ConvexQuad::distance(const Point &point) const
{
	return std::min(
	    Triangle{{points[0], points[1], points[2]}}.distance(point),
	    Triangle{{points[2], points[3], points[0]}}.distance(point),
	    Math::Floating::less);
}

}