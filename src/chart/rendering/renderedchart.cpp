#include "renderedchart.h"

#include <ranges>

#include "chart/rendering/markerrenderer.h"

using namespace Vizzu::Draw;

void RenderedChart::add(DrawingElement &&element)
{
	elements.push_back(std::move(element));
}

void Vizzu::Draw::RenderedChart::hintAddCount(size_t count)
{
	elements.reserve(elements.size() + count);
}

const DrawingElement &RenderedChart::find(const Geom::Point &point) const
{
	auto original = coordinateSystem.getOriginal(point);

	for (auto &element : std::ranges::reverse_view(elements))
	{
		if (auto *rect = std::get_if<Geom::TransformedRect>(&element.geometry))
		{
			if (rect->contains(point)) return element;
		}
		else if (auto *line = std::get_if<Line>(&element.geometry))
		{
			const auto p = line->usesBaseTransform ? original : point;
			auto maxDistance = line->usesBaseTransform ? 0.1 : 10.0;
			if (line->line.distance(p) <= maxDistance) return element;
		}
		else if (auto *rect = std::get_if<Rect>(&element.geometry))
		{
			const auto p = rect->usesBaseTransform ? original : point;
			if (rect->rect.contains(p))
				return element;
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

				if (drawItem.bounds(original)) return element;
			}
		}
	}
	return elements.front();
}