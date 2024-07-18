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
		    case Gen::ShapeType::rectangle:
		    case Gen::ShapeType::area:
			    return Geom::ConvexQuad(points).contains(point, 0.01);
		    case Gen::ShapeType::circle:
			    return Geom::Circle(Geom::Rect::Boundary(points),
			        Geom::Circle::FromRect::sameWidth)
			        .overlaps(Geom::Circle(point, 0.01), 0.1);
		    case Gen::ShapeType::line:
			    return lineToQuad(coordSys, 10.0)
			        .contains(coordSys.convert(point), 0.1);
		    }
		    return false;
	    });

	return isInside != false;
}

Geom::ConvexQuad Marker::lineToQuad(const CoordinateSystem &coordSys,
    double atLeastWidth) const
{
	auto line = getLine();

	auto pBeg = coordSys.convert(line.begin);
	auto pEnd = coordSys.convert(line.end);

	auto wBeg = lineWidth[0] * coordSys.getRect().size.minSize();
	auto wEnd = lineWidth[1] * coordSys.getRect().size.minSize();
	return Geom::ConvexQuad::Isosceles(pBeg,
	    pEnd,
	    std::max(atLeastWidth, wBeg * 2),
	    std::max(atLeastWidth, wEnd * 2));
}
Geom::Line Marker::getLine() const { return {points[3], points[2]}; }
}