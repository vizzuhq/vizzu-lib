#ifndef CHART_SPECMARKER_H
#define CHART_SPECMARKER_H

#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/geom/circle.h"

namespace Vizzu
{
namespace Charts
{

struct SpecMarker
{
	size_t index;
	double size;
	Geom::Point pos0;
	Geom::Point pos1;

	SpecMarker(size_t index, double size) {
		this->index = index;
		this->size = size;
	}

	SpecMarker(size_t index, const Geom::Circle &circle) {
		this->index = index;
		pos0 = circle.center;
		pos1 = circle.center + Geom::Point(circle.radius, 0); 
	}

	SpecMarker(size_t index, const Geom::Point &p0, const Geom::Point &p1) {
		this->index = index;
		pos0 = p0;
		pos1 = p1;
	}

	Geom::Rect rect() const { return Geom::Rect(pos0, pos1 - pos0); }
	Geom::Circle circle() const { return Geom::Circle(pos0, (pos1 - pos0).x); }

	static bool indexOrder(const SpecMarker &a, const SpecMarker &b)
	{
		return a.index < b.index;
	}

	static bool sizeOrder(const SpecMarker &a, const SpecMarker &b)
	{
		return b.size < a.size;
	}
};

}
}

#endif
