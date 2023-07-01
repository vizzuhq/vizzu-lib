#include "renderedchart.h"

#include <ranges>

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
	for (auto &element : std::ranges::reverse_view(elements))
	{
		if (auto *rect = std::get_if<Geom::TransformedRect>(&element.geometry);
			rect->contains(point))
			return element;
	}
	return elements.front();
}