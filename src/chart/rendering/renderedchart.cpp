#include "renderedchart.h"

#include <ranges>

#include "chart/rendering/markerrenderer.h"

namespace Vizzu::Draw
{

const Util::EventTarget *RenderedChart::find(
    const Geom::Point &point) const
{
	auto original = coordinateSystem.getOriginal(point);

	for (const auto &element : std::ranges::reverse_view(elements)) {
		if (const auto *rect = std::get_if<Geom::TransformedRect>(
		        &element.geometry)) {
			if (rect->contains(point)) return element.target.get();
		}
		else if (const auto *line =
		             std::get_if<Line>(&element.geometry)) {
			const auto p = line->relative ? original : point;
			auto maxDistance = line->relative ? 0.01 : 10.0;
			if (line->line.distance(p) <= maxDistance)
				return element.target.get();
		}
		else if (const auto *rect =
		             std::get_if<Rect>(&element.geometry)) {
			const auto p = rect->relative ? original : point;
			if (rect->rect.contains(p)) return element.target.get();
		}
		else if (const auto *marker =
		             std::get_if<Marker>(&element.geometry)) {
			if (marker->bounds(coordinateSystem, original))
				return element.target.get();
		}
	}
	return nullptr;
}

bool Marker::bounds(const CoordinateSystem &coordSys,
    const Geom::Point &point) const
{
	if (!enabled) return false;

	/** Approximated solution */
	auto isInside = shapeType.combine<Math::FuzzyBool>(
	    [this, &point, &coordSys](const Gen::ShapeType &shapeType)
	    {
		    switch (shapeType) {
			    using enum Gen::ShapeType;
		    case rectangle:
		    case area:
			    return Geom::ConvexQuad{points}.distance(point)
			         < 0.01;
		    case circle: {
			    auto &&rect = Geom::Rect::Boundary(points);
			    return Geom::Circle(rect.pos + rect.size / 2.0,
			               rect.size.x / 2.0)
			               .distance(point)
			         < 0.01;
		    }
		    case line:
			    return lineToQuad(coordSys).distance(
			               coordSys.convert(point))
			         < 0.01;
		    }
		    return false;
	    });

	return isInside != false;
}

Geom::ConvexQuad Marker::lineToQuad(
    const CoordinateSystem &coordSys) const
{
	using Math::Floating::less;
	auto line = getLine();

	auto pBeg = coordSys.convert(line.begin);
	auto pEnd = coordSys.convert(line.end);

	auto wBeg = lineWidth[0] * coordSys.getRect().size.minSize();
	auto wEnd = lineWidth[1] * coordSys.getRect().size.minSize();
	return Geom::ConvexQuad::Isosceles(pBeg,
	    pEnd,
	    wBeg * 2,
	    wEnd * 2);
}

Geom::Line Marker::getLine() const { return {points[3], points[2]}; }
}