#include "triangle.h"

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
	const auto A = points[2] - points[1];
	const auto B = points[0] - points[1];
	const auto C = points[2] - points[0];
	const auto dA = Geom::Line{points[1], points[2]}.distance(point);
	const auto dB = Geom::Line{points[0], points[1]}.distance(point);
	const auto dC = Geom::Line{points[2], points[0]}.distance(point);
	if (const auto double_area = fabs(A ^ B);
	    !Math::Floating::is_zero(double_area)
	    && Math::AddTolerance(
	           A.abs() * dA + B.abs() * dB + C.abs() * dC)
	           == double_area) {
		return 0.0;
	}
	return std::min({dA, dB, dC}, Math::Floating::less);
}

}