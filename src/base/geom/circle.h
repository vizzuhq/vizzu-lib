#ifndef GEOM_CIRCLE
#define GEOM_CIRCLE

#include <cstdint>

#include "point.h"
#include "rect.h"
#include "solutions.h"

namespace Geom
{

class Circle
{
public:
	enum class FromRect : std::uint8_t {
		inscribed,
		sameWidth,
		sameHeight,
		outscribed
	};

	Point center;
	double radius;

	Circle() : radius{} {}

	Circle(Point center, double radius) :
	    center(center),
	    radius(radius)
	{}

	Circle(const Rect &rect, FromRect fromRect);

	Circle(const Circle &c0,
	    const Circle &c1,
	    double radius,
	    double sign);

	Circle operator*(double factor) const
	{
		return {center * factor, radius * factor};
	}

	Circle operator+(const Point &offset) const
	{
		return {center + offset, radius};
	}

	[[nodiscard]] double area() const;
	[[nodiscard]] bool overlaps(const Circle &c,
	    double tolerance) const;
	[[nodiscard]] double overlapFactor(const Circle &c) const;
	[[nodiscard]] bool colateral(const Circle &c,
	    double tolerance) const;
	[[nodiscard]] Rect boundary() const;
	[[nodiscard]] bool contains(const Point &point) const;
	[[nodiscard]] double distance(const Circle &c) const;
	[[nodiscard]] Solutions<Point, 2> intersection(
	    const Circle &c) const;
	[[nodiscard]] double signedDistance(const Circle &c) const;
	[[nodiscard]] double centerDistance(const Circle &c) const;
	[[nodiscard]] bool concentric(const Circle &c) const;
};

}

#endif
