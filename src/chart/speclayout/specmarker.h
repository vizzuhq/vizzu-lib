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
	std::variant<double, Geom::Rect, Geom::Circle> sizeOrShape;

	SpecMarker(size_t index, double radiusOrAreaFactorSize) :
	    index(index),
	    sizeOrShape(radiusOrAreaFactorSize)
	{}

	template <typename... T> void emplaceCircle(T &&...params)
	{
		sizeOrShape.emplace<Geom::Circle>(std::forward<T>(params)...);
	}

	void emplaceRect(const Geom::Point &p0, const Geom::Point &p1)
	{
		sizeOrShape.emplace<Geom::Rect>(p0, Geom::Size{p1 - p0});
	}

	[[nodiscard]] const double &size() const
	{
		return *std::get_if<double>(&sizeOrShape);
	}

	[[nodiscard]] const Geom::Rect &rect() const
	{
		return *std::get_if<Geom::Rect>(&sizeOrShape);
	}
	[[nodiscard]] const Geom::Circle &circle() const
	{
		return *std::get_if<Geom::Circle>(&sizeOrShape);
	}

	static bool indexOrder(const SpecMarker &a, const SpecMarker &b)
	{
		return a.index < b.index;
	}

	static bool sizeOrder(const SpecMarker &a, const SpecMarker &b)
	{
		return b.size() < a.size();
	}
};

}

#endif
