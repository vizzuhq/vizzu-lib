#include "point.h"

namespace Geom
{

Size Size::rotatedSize(double angle) const
{
	Size res;
	auto c = ::fabs(::cos(angle));
	auto s = ::fabs(::sin(angle));
	res.x = x * c + y * s;
	res.y = x * s + y * c;
	return res;
}

Point Point::rotated(double angle) const
{
	return {cos(angle) * x - sin(angle) * y,
	    sin(angle) * x + cos(angle) * y};
}

Point Point::normalized() const
{
	using Math::Floating::is_zero;
	auto length = abs();
	if (is_zero(length)) return {0, 0};
	return *this / length;
}

Point Point::normal(bool clockwise) const
{
	if (clockwise) return {y, -x};
	return {-y, x};
}

}