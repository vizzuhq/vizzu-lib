#include "triangle.h"

#include <compare>

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
	if (auto r1 = rot_dir(points[0], points[1], point),
	    r2 = rot_dir(points[1], points[2], point),
	    r3 = rot_dir(points[2], points[0], point);
	    (is_neq(r1) || is_neq(r2) || is_neq(r3))
	    && ((is_lteq(r1) && is_lteq(r2) && is_lteq(r3))
	        || (is_gteq(r1) && is_gteq(r2) && is_gteq(r3))))
		return 0.0;

	return std::min({Line{points[0], points[1]}.distance(point),
	                    Line{points[1], points[2]}.distance(point),
	                    Line{points[2], points[0]}.distance(point)},
	    Math::Floating::less);
}

}