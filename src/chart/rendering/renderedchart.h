#ifndef CHART_RENDERING_RENDEREDCHART_H
#define CHART_RENDERING_RENDEREDCHART_H

#include <vector>
#include <variant>

#include "base/geom/affinetransform.h"
#include "base/geom/transformedrect.h"
#include "base/geom/quadrilateral.h"
#include "base/geom/line.h"
#include "base/geom/circle.h"
#include "base/util/eventdispatcher.h"

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

class DrawingElement {
public:
	typedef std::variant<
		Geom::TransformedRect,
		Line
	> Geometry;

	template <class T>
	DrawingElement(const T &geometry) : geometry(geometry)
	{}

	DrawingElement(/*const Util::EventTarget &target*/)/* : target(target)*/
	{}

	Geometry geometry;

private:
//	const Util::EventTarget &target;
};

class RenderedChart
{
public:
	RenderedChart() = default;
	RenderedChart(const CoordinateSystem &coordinateSystem)
		: coordinateSystem(coordinateSystem) {}

	template <typename ... T>
	void emplace(T &&...args) {
		elements.emplace_back(std::forward<T>(args)...);
	}

	void add(DrawingElement &&element);
	void hintAddCount(size_t count);

	const DrawingElement &find(const Geom::Point &point) const;

private:
	CoordinateSystem coordinateSystem;
	std::vector<DrawingElement> elements;

};

}
}

#endif
