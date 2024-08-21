#include "renderedchart.h"

#include <algorithm>
#include <limits>
#include <ranges>
#include <variant>

#include "base/geom/point.h"
#include "base/geom/transformedrect.h"
#include "base/math/floating.h"
#include "base/util/eventdispatcher.h"
#include "chart/generator/marker.h" // NOLINT(misc-include-cleaner)
#include "chart/options/shapetype.h"

namespace Vizzu::Draw
{

const Util::EventTarget *RenderedChart::find(
    const Geom::Point &point) const
{
	auto original = coordinateSystem.getOriginal(point);

	const Util::EventTarget *closestMarker{};
	double closestMarkerDistance = std::numeric_limits<double>::max();

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
			constexpr double MARKER_DISTANCE_THRESHOLD = 0.01;
			if (const auto dist =
			        marker->distance(coordinateSystem, original);
			    dist < MARKER_DISTANCE_THRESHOLD
			    && dist < closestMarkerDistance) {
				closestMarker = element.target.get();
				if (Math::Floating::is_zero(dist)) break;
				closestMarkerDistance = dist;
			}
		}
	}
	return closestMarker;
}

double Marker::distance(const CoordinateSystem &coordSys,
    const Geom::Point &point) const
{
	if (!enabled) return std::numeric_limits<double>::max();

	/** Approximated solution */
	return shapeType.combine(
	    [this, &point, &coordSys](const Gen::ShapeType &shapeType)
	    {
		    switch (shapeType) {
			    using enum Gen::ShapeType;
		    default:
		    case rectangle:
		    case area:
			    return Geom::ConvexQuad{points}.distance(point);
		    case circle: {
			    auto &&rect = Geom::Rect::Boundary(points);
			    return Geom::Circle(rect.pos + rect.size / 2.0,
			        rect.size.x / 2.0)
			        .distance(point);
		    }
		    case line: {
			    auto rel_to_px = coordSys.getRect().size.minSize();
			    return lineToQuad(coordSys).distance(
			               coordSys.convert(point))
			         / (Math::Floating::is_zero(rel_to_px)
			                 ? 1.0
			                 : rel_to_px);
		    }
		    }
	    });
}

Geom::ConvexQuad Marker::lineToQuad(
    const CoordinateSystem &coordSys) const
{
	constexpr double MIN_WIDTH_PX = 10.0;
	auto line = getLine();

	auto pBeg = coordSys.convert(line.begin);
	auto pEnd = coordSys.convert(line.end);

	auto wBeg = lineWidth[0] * coordSys.getRect().size.minSize();
	auto wEnd = lineWidth[1] * coordSys.getRect().size.minSize();

	using Math::Floating::less;
	return Geom::ConvexQuad::Isosceles(pBeg,
	    pEnd,
	    std::max(MIN_WIDTH_PX, wBeg * 2, less),
	    std::max(MIN_WIDTH_PX, wEnd * 2, less));
}

Geom::Line Marker::getLine() const { return {points[3], points[2]}; }
}