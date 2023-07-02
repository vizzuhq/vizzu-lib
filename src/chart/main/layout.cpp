#include "layout.h"

#include "chart/rendering/drawlabel.h"

using namespace Vizzu;

void Layout::setBoundary(const Geom::Rect &boundary, Gfx::ICanvas &)
{
	this->boundary = boundary;
}

void Layout::setBoundary(const Geom::Rect &boundary,
    const Gen::Plot &plot,
    Gfx::ICanvas &info)
{
	auto &style = plot.getStyle();
	auto em = style.Font::calculatedSize();

	this->boundary = boundary;
	auto rect = style.contentRect(boundary, em);

	auto titleHeight = Draw::drawLabel::getHeight(style.title, info);

	auto titlePos = plot.getOptions()->title.combine<double>(
	    [&](int, const auto &title)
	    {
		    return title ? 0 : -titleHeight;
	    });

	title = rect.popBottom(titlePos + titleHeight);
	title.setBottom(titlePos);

	auto legendWidth = style.legend.computedWidth(rect.size.x, em);

	auto legendPos = plot.getOptions()->legend.combine<double>(
	        [&](int, const auto &legend)
	        {
		        return legend ? 0 : -legendWidth;
	        });

	auto legenPosBase = rect.pos.x;
	legend = rect.popLeft(legendPos + legendWidth);
	legend.setLeft(legenPosBase + legendPos);

	this->plot = rect;

	plotArea = style.plot.contentRect(rect, em);
}
