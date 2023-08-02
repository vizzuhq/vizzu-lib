#include "circle.h"

#include <algorithm>

#include "base/math/tolerance.h"

using namespace Geom;

Circle::Circle(const Rect &rect, FromRect fromRect)
{
	radius = fromRect == FromRect::inscribed
	           ? std::min(rect.size.x, rect.size.y) / 2.0
	       : fromRect == FromRect::sameWidth  ? rect.size.x / 2.0
	       : fromRect == FromRect::sameHeight ? rect.size.y / 2.0
	       : fromRect == FromRect::outscribed
	           ? rect.size.diagonal() / 2.0
	           : throw std::logic_error("invalid circle parameter");
	center = rect.pos + rect.size / 2.0;
}

Circle::Circle(const Circle &c0,
    const Circle &c1,
    double radius,
    double sign)
{
	auto d01 = (c0.center - c1.center).abs();
	auto d12 = c1.radius + radius;
	auto d20 = c0.radius + radius;
	auto phiA1 =
	    acos((d20 * d20 + d01 * d01 - d12 * d12) / (2 * d20 * d01));
	auto phiA0 = (c1.center - c0.center).angle();
	center = c0.center
	       + Point::Polar(c0.radius + radius, phiA0 + sign * phiA1);
	this->radius = radius;
}

bool Circle::concentric(const Circle &c) const
{
	return center == c.center;
}

bool Circle::colateral(const Circle &c, double tolerance) const
{
	return Math::AddTolerance(centerDistance(c), tolerance)
	    == (radius + c.radius);
}

double Circle::area() const { return M_PI * radius * radius; }

bool Circle::overlaps(const Circle &c, double tolerance) const
{
	auto d = c.center - center;
	auto sumRadius = radius + c.radius;
	return Math::AddTolerance(d.sqrAbs(), tolerance)
	     < sumRadius * sumRadius;
}

double Circle::overlapFactor(const Circle &c) const
{
	auto d = centerDistance(c);
	auto r = radius + c.radius;
	return d == 0 ? 0 : r / d;
}

Rect Circle::boundary() const
{
	return {center - Point(1, 1) * radius,
	    Size(2 * radius, 2 * radius)};
}

bool Circle::contains(const Point &point) const
{
	return (point - center).sqrAbs() <= radius * radius;
}

double Circle::centerDistance(const Circle &c) const
{
	return (center - c.center).abs();
}

double Circle::distance(const Circle &c) const
{
	return std::max(0.0, centerDistance(c) - radius - c.radius);
}

double Circle::signedDistance(const Circle &c) const
{
	return centerDistance(c) - (radius + c.radius);
}

Solutions<Point, 2> Circle::intersection(const Circle &c) const
{
	if (concentric(c)) return {};

	auto radicalLineDistance =  (std::pow(radius, 2) - std::pow(c.radius, 2)
	           + std::pow(centerDistance(c), 2))
	     / (2 * centerDistance(c));

	if (radicalLineDistance > radius) return {};

	const Point directionVector = (c.center - center).normalized();

	auto radicalPoint = center + directionVector * radicalLineDistance;

	if (radicalLineDistance == radius)
		return { radicalPoint, std::nullopt };

	auto radicalLineHeight = 
	    std::sqrt(std::pow(radius, 2)
	    - std::pow(radicalLineDistance, 2));

	return {
		radicalPoint + directionVector.leftNormal() * radicalLineHeight,
		radicalPoint + directionVector.rightNormal() * radicalLineHeight,
	};
}
