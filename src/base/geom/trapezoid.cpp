#include "trapezoid.h"

#include "base/math/interpolation.h"
#include "base/math/range.h"

using namespace Geom;

bool VerticalTrapezoid::contains(const Point &p) const
{
	if (p.x < points[0].x || p.x > points[1].x) return false;

	auto xf =
	    Math::Range<double>(points[0].x, points[1].x).rescale(p.x);
	auto y0 = Math::interpolate(points[0].y, points[1].y, xf);
	auto y1 = Math::interpolate(points[3].y, points[2].y, xf);

	return Math::Range<double>(y0, y1).includes(p.y);
}
