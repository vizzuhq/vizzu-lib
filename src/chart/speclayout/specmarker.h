#ifndef CHART_SPECMARKER_H
#define CHART_SPECMARKER_H

#include <variant>

#include "base/geom/circle.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"

namespace Vizzu::Charts
{

struct SpecMarker
{
	size_t index;
	double size;
	std::variant<Geom::Rect, Geom::Circle> shape;

	SpecMarker(size_t index, double radiusOrAreaFactor) :
	    index(index),
	    size(radiusOrAreaFactor)
	{}

	template <typename... T> void emplaceCircle(T &&...params)
	{
		shape.emplace<Geom::Circle>(std::forward<T>(params)...);
	}

	void emplaceRect(const Geom::Point &p0, const Geom::Point &p1)
	{
		shape.emplace<Geom::Rect>(p0, Geom::Size{p1 - p0});
	}

	[[nodiscard]] const Geom::Rect &rect() const
	{
		return *std::get_if<Geom::Rect>(&shape);
	}
	[[nodiscard]] const Geom::Circle &circle() const
	{
		return *std::get_if<Geom::Circle>(&shape);
	}

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

#endif
