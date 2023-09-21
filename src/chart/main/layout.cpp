#include "layout.h"

#include "chart/rendering/drawlabel.h"

namespace Vizzu
{

void Layout::setBoundary(const Geom::Rect &boundary, Gfx::ICanvas &)
{
	this->boundary = boundary;
}

template <class T>
auto popRectArea(double height,
    ::Anim::Interpolated<T> &interpolated,
    Geom::Rect &rect,
    Geom::Rect (Geom::Rect::*pop)(double),
    void (Geom::Rect::*set)(double) = nullptr,
    double plus = 0.0)
{
	auto pos = interpolated.template combine<double>(
	    [&](int, const auto &weight)
	    {
		    return weight ? 0 : -height;
	    });

	auto &&res = std::invoke(pop, rect, pos + height);
	if (set) { std::invoke(set, res, pos + plus); }
	return res;
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

	footer = Geom::Rect{rect}.popTop(
	    plot.getOptions()->footer.combine<double>(
	        [&](int, const auto &weight)
	        {
		        return weight ? 0 : -footerHeight;
	        })
	    + footerHeight);

	title = popRectArea(Draw::DrawLabel::getHeight(style.title, info),
	    plot.getOptions()->title,
	    rect,
	    &Geom::Rect::popBottom,
	    &Geom::Rect::setBottom);

	subtitle =
	    popRectArea(Draw::DrawLabel::getHeight(style.subtitle, info),
	        plot.getOptions()->subtitle,
	        rect,
	        &Geom::Rect::popBottom,
	        &Geom::Rect::setBottom,
	        rect.pos.y);

	legend = popRectArea(style.legend.computedWidth(rect.size.x, em),
	    plot.getOptions()->legend,
	    rect,
	    &Geom::Rect::popLeft,
	    &Geom::Rect::setLeft,
	    rect.pos.x);

	this->plot = rect;

	plotArea = style.plot.contentRect(rect, em);
}

}