#include "layout.h"

#include "chart/rendering/drawlabel.h"

namespace Vizzu
{

void Layout::setBoundary(const Geom::Rect &boundary, Gfx::ICanvas &)
{
	this->boundary = boundary;
}

void Layout::setBoundary(const Geom::Rect &boundary,
    const Gen::Plot &plot,
    Gfx::ICanvas &info)
{
	const auto &style = plot.getStyle();
	auto em = style.calculatedSize();

	this->boundary = boundary;
	auto rect = style.contentRect(boundary, em);

	auto footerHeight =
	    Draw::DrawLabel::getHeight(style.footer, info);

	auto footerPos = plot.getOptions()->footer.combine<double>(
	    [&](int, const auto &footer)
	    {
		    return footer ? 0 : -footerHeight;
	    });

	footer = rect.popTop(footerPos + footerHeight);

	auto titleHeight = Draw::DrawLabel::getHeight(style.title, info);

	auto titlePos = plot.getOptions()->title.combine<double>(
	    [&](int, const auto &title)
	    {
		    return title ? 0 : -titleHeight;
	    });

	title = rect.popBottom(titlePos + titleHeight);
	title.setBottom(titlePos);

	auto subtitleHeight =
	    Draw::DrawLabel::getHeight(style.subtitle, info);

	auto subtitlePos = plot.getOptions()->subtitle.combine<double>(
	    [&](int, const auto &subtitle)
	    {
		    return subtitle ? 0 : -subtitleHeight;
	    });

	subtitle = rect.popBottom(subtitlePos + subtitleHeight);
	subtitle.setBottom(titlePos + titleHeight + subtitlePos);

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

}