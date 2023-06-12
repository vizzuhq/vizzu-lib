#ifndef GEOM_CIRCLE
#define GEOM_CIRCLE

#include "point.h"
#include "rect.h"

namespace Geom
{

class Circle
{
public:
	enum class FromRect {
		inscribed,
		sameWidth,
		sameHeight,
		outscribed
	};

	Point center;
	double radius;

	Circle() : radius(0) {}

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
		return Circle(center * factor, radius * factor);
	}

	Circle operator+(const Point &offset) const
	{
		return Circle(center + offset, radius);
	}

	double area() const;
	bool overlaps(const Circle &c, double tolerance) const;
	double overlapFactor(const Circle &c) const;
	bool colateral(const Circle &c, double tolerance) const;
	Rect boundary() const;
	bool contains(const Point &point) const;
	double distance(const Circle &c) const;
	Point intersection(const Circle &c) const;
	double radicalLineHeight(const Circle &c) const;
	double radicalLineDistance(const Circle &c) const;
	double signedDistance(const Circle &c) const;
	double centerDistance(const Circle &c) const;
	bool concentric(const Circle &c) const;
};

}

#endif
