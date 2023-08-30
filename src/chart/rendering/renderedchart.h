#ifndef CHART_RENDERING_RENDEREDCHART_H
#define CHART_RENDERING_RENDEREDCHART_H

#include <vector>
#include <variant>
#include <memory>

#include "base/geom/affinetransform.h"
#include "base/geom/transformedrect.h"
#include "base/geom/quadrilateral.h"
#include "base/geom/line.h"
#include "base/geom/circle.h"
#include "base/util/eventdispatcher.h"

#include "chart/generator/plot.h"
#include "chart/rendering/painter/coordinatesystem.h"

namespace Vizzu
{

namespace Draw
{

class Line
{
public:
	Geom::Line line;
	bool usesBaseTransform;
};

class Rect
{
public:
	Geom::Rect rect;
	bool usesBaseTransform;
};

class Marker
{
public:
	const Gen::Marker &marker;
};

class DrawingElement {
public:
	typedef std::variant<
		Geom::TransformedRect,
		Line,
		Rect,
		Marker
	> Geometry;

	template <class T>
	DrawingElement(
		const T &geometry, 
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
		const Gen::Plot *plot = nullptr) :
		coordinateSystem(coordinateSystem),
		plot(plot)
	{}

	template <typename ... T>
	void emplace(T &&...args) {
		elements.emplace_back(std::forward<T>(args)...);
	}

	const Util::EventTarget *find(const Geom::Point &point) const;

private:
	CoordinateSystem coordinateSystem;
	const Gen::Plot *plot;
	std::vector<DrawingElement> elements;

};

}
}

#endif
