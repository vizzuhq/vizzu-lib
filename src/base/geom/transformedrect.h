#ifndef GEOM_TRANSFORMEDRECT
#define GEOM_TRANSFORMEDRECT

#include "affinetransform.h"
#include "rect.h"

namespace Geom
{

class TransformedRect
{
public:
	AffineTransform transform;
	Size size;

	static TransformedRect fromRect(const Rect &rect)
	{
		return { AffineTransform(rect.pos), rect.size };
	}

	[[nodiscard]] bool contains(const Point &point) const
	{
		auto transformed = transform.inverse()(point);
		return Rect(Point(), size).contains(transformed);
	}
};

}

#endif
