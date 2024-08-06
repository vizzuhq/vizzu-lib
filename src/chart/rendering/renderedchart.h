#ifndef CHART_RENDERING_RENDEREDCHART_H
#define CHART_RENDERING_RENDEREDCHART_H

#include <memory>
#include <variant>
#include <vector>

#include "base/geom/affinetransform.h"
#include "base/geom/circle.h"
#include "base/geom/line.h"
#include "base/geom/quadrilateral.h"
#include "base/geom/transformedrect.h"
#include "base/util/eventdispatcher.h"
#include "chart/generator/plotptr.h"
#include "chart/options/shapetype.h"
#include "chart/rendering/painter/coordinatesystem.h"

namespace Vizzu::Draw
{

class Line
{
public:
	Geom::Line line;
	bool relative;
};

class Rect
{
public:
	Geom::Rect rect;
	bool relative;
};

class Marker
{
public:
	bool enabled;
	::Anim::Interpolated<Gen::ShapeType> shapeType;
	std::array<Geom::Point, 4> points;
	std::array<double, 2> lineWidth;

	[[nodiscard]] bool bounds(const CoordinateSystem &coordSys,
	    const Geom::Point &point) const;

private:
	[[nodiscard]] Geom::ConvexQuad lineToQuad(
	    const CoordinateSystem &coordSys) const;

	[[nodiscard]] Geom::Line getLine() const;
};

class DrawingElement
{
public:
	using Geometry =
	    std::variant<Geom::TransformedRect, Line, Rect, Marker>;

	template <class T>
	DrawingElement(const T &geometry,
	    std::unique_ptr<Util::EventTarget> target) :
	    geometry(geometry),
	    target(std::move(target))
	{}

	Geometry geometry;
	std::unique_ptr<Util::EventTarget> target;
};

class RenderedChart
{
public:
	RenderedChart() = default;
	explicit RenderedChart(const CoordinateSystem &coordinateSystem,
	    std::shared_ptr<const Gen::Plot> plot) :
	    coordinateSystem(coordinateSystem),
	    plot(std::move(plot))
	{}

	template <typename... T> void emplace(T &&...args)
	{
		elements.emplace_back(std::forward<T>(args)...);
	}

	[[nodiscard]] const Util::EventTarget *find(
	    const Geom::Point &point) const;

	[[nodiscard]] const CoordinateSystem &getCoordSys() const
	{
		return coordinateSystem;
	}

private:
	CoordinateSystem coordinateSystem;
	std::shared_ptr<const Gen::Plot> plot;
	std::vector<DrawingElement> elements;
};

}

#endif
