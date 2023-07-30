#include "point.h"

using namespace Geom;

Size Size::UpperIdentity(double aspectRatio)
{
	if (aspectRatio >= 1.0)
		return {aspectRatio, 1.0};
	else
		return {1.0, 1.0 / aspectRatio};
}

Size Size::LowerIdentity(double aspectRatio)
{
	if (aspectRatio <= 1.0)
		return {aspectRatio, 1.0};
	else
		return {1.0, 1.0 / aspectRatio};
}

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
	auto length = abs();
	if (length == 0.0)
		return {0, 0};
	else
		return *this / length;
}

Point Point::normal(bool clockwise) const
{
	if (clockwise)
		return {y, -x};
	else
		return {-y, x};
}
