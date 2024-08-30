#include "triangle.h"

#include <algorithm>
#include <cmath>
#include <compare>

#include "base/math/floating.h"

#include "line.h"

namespace Geom
{

double Triangle::area() const
{
	auto A = points[2] - points[1];
	auto B = points[0] - points[1];
	return fabs(A ^ B) / 2;
}

double Triangle::distance(const Point &point) const
{
	auto rot_dir = [](const Point &a, const Point &b, const Point &c)
	{
		return std::weak_order(0.0,
		    (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
	};

	using std::is_gteq;
	using std::is_lteq;
	using std::is_neq;
	const auto r1 = rot_dir(points[0], points[1], point);
	const auto r2 = rot_dir(points[1], points[2], point);
	const auto r3 = rot_dir(points[2], points[0], point);

	if (const auto non_singular =
	        is_neq(r1) || is_neq(r2) || is_neq(r3),
	    rotate_to_same_dir =
	        (is_lteq(r1) && is_lteq(r2) && is_lteq(r3))
	        || (is_gteq(r1) && is_gteq(r2) && is_gteq(r3));
	    non_singular && rotate_to_same_dir)
		return 0.0;

	return std::min({Line{points[0], points[1]}.distance(point),
	                    Line{points[1], points[2]}.distance(point),
	                    Line{points[2], points[0]}.distance(point)},
	    Math::Floating::less);
}

}