#include "layout.h"

#include "chart/rendering/drawlabel.h"

using namespace Vizzu;

void Layout::setBoundary(
	const Geom::Rect &boundary,
	Gfx::ICanvas &)
{
	this->boundary = boundary;
}

void Layout::setBoundary(const Geom::Rect &boundary,
    const Diag::Diagram &diagram,
    Gfx::ICanvas &info)
{
	auto &style = diagram.getStyle();
	auto em = style.Font::calculatedSize();

	this->boundary = boundary;
	auto rect = style.contentRect(boundary, em);

	auto titleHeight = Draw::drawLabel::getHeight(style.title, info);

	auto titlePos = diagram.getOptions()->title.get().combine<double>(
	    [&](const auto &title) { return title ? 0 : -titleHeight; });

	title = rect.popBottom(titlePos + titleHeight);
	title.setBottom(titlePos);

	auto legendWidth = style.legend.computedWidth(rect.size.x, em);

	auto legendPos = diagram.getOptions()->legend.get().combine<double>(
	    [&](const auto &legend) { return legend ? 0 : -legendWidth; });

	legend = rect.popLeft(legendPos + legendWidth);
	legend.setLeft(legendPos);

	plot = rect;

	plotArea = style.plot.contentRect(rect, em);
}
