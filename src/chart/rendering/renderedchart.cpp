#include "renderedchart.h"

#include <ranges>

#include "chart/rendering/markerrenderer.h"

using namespace Vizzu::Draw;

const Util::EventTarget *RenderedChart::find(const Geom::Point &point) const
{
	auto original = coordinateSystem.getOriginal(point);

	for (auto &element : std::ranges::reverse_view(elements))
	{
		if (auto *rect = std::get_if<Geom::TransformedRect>(&element.geometry))
		{
			if (rect->contains(point)) 
				return element.target.get();
		}
		else if (auto *line = std::get_if<Line>(&element.geometry))
		{
			const auto p = line->relative ? original : point;
			auto maxDistance = line->relative ? 0.01 : 10.0;
			if (line->line.distance(p) <= maxDistance) 
				return element.target.get();
		}
		else if (auto *rect = std::get_if<Rect>(&element.geometry))
		{
			const auto p = rect->relative ? original : point;
			if (rect->rect.contains(p))
				return element.target.get();
		}
		else if (auto *marker = std::get_if<Marker>(&element.geometry))
		{
			if (plot) {
				const auto &options = *plot->getOptions();

				auto drawItem = Draw::AbstractMarker::createInterpolated(
					marker->marker,
					options,
					plot->getStyle(),
					coordinateSystem,
					plot->getMarkers(),
					0);

				if (drawItem.bounds(original)) 
					return element.target.get()	;
			}
		}
	}
	return nullptr;
}