#include "point.h"

Geom::Size Geom::Size::UpperIdentity(double aspectRatio)
{
	if (aspectRatio >= 1.0) return {aspectRatio, 1.0};
	return {1.0, 1.0 / aspectRatio};
}

Geom::Size Geom::Size::LowerIdentity(double aspectRatio)
{
	if (aspectRatio <= 1.0) return {aspectRatio, 1.0};
	return {1.0, 1.0 / aspectRatio};
}

Geom::Size Geom::Size::rotatedSize(double angle) const
{
	Size res;
	auto c = ::fabs(::cos(angle));
	auto s = ::fabs(::sin(angle));
	res.x = x * c + y * s;
	res.y = x * s + y * c;
	return res;
}

Geom::Point Geom::Point::rotated(double angle) const
{
	return {cos(angle) * x - sin(angle) * y,
	    sin(angle) * x + cos(angle) * y};
}

Geom::Point Geom::Point::normalized() const
{
	auto length = abs();
	if (length == 0.0) return {0, 0};
	return *this / length;
}

Geom::Point Geom::Point::normal(bool clockwise) const
{
	if (clockwise) return {y, -x};
	return {-y, x};
}
