#include "layout.h"

#include "chart/rendering/drawlabel.h"
#include "chart/rendering/logo.h"

#include "stylesheet.h"

namespace Vizzu
{

template <class T>
auto popRectArea(double height,
    ::Anim::Interpolated<T> &interpolated,
    Geom::Rect &rect,
    Geom::Rect (Geom::Rect::*pop)(double),
    void (Geom::Rect::*set)(double) = nullptr,
    double plus = 0.0)
{
	auto pos = interpolated.template combine<double>(
	    [&height](const auto &weight)
	    {
		    return weight ? 0 : -height;
	    });

	auto &&res = std::invoke(pop, rect, pos + height);
	if (set) { std::invoke(set, res, pos + plus); }
	return res;
}

void Layout::setBoundary(const Geom::Rect &boundary,
    const Styles::Chart &style,
    const Gen::PlotOptionsPtr &options)
{
	this->boundary = boundary;

	setLogoBoundary(style.logo);

	if (!options) return;

	auto em = style.calculatedSize();
	auto rect = style.contentRect(boundary, em);

	caption = popRectArea(style.caption.getHeight(),
	    options->caption,
	    rect,
	    &Geom::Rect::popTop);
	caption.setBottom(rect.top());
	caption.setTop(boundary.top());

	title = popRectArea(style.title.getHeight(),
	    options->title,
	    rect,
	    &Geom::Rect::popBottom,
	    &Geom::Rect::setBottom,
	    rect.pos.y);

	subtitle = popRectArea(style.subtitle.getHeight(),
	    options->subtitle,
	    rect,
	    &Geom::Rect::popBottom,
	    &Geom::Rect::setBottom,
	    rect.pos.y);

	legend = popRectArea(style.legend.computedWidth(rect.size.x, em),
	    options->legend,
	    rect,
	    &Geom::Rect::popLeft,
	    &Geom::Rect::setLeft,
	    rect.pos.x);

	this->plot = rect;

	plotArea = style.plot.contentRect(rect, em);
}

void Layout::setLogoBoundary(const Styles::Logo &logoStyle)
{
	auto fontSize = Styles::Sheet::baseFontSize(boundary.size, false);
	auto logoWidth =
	    logoStyle.width->get(boundary.size.minSize(), fontSize);

	auto logoHeight = Draw::Logo::height(logoWidth);

	auto logoPad =
	    logoStyle.toInvMargin(Geom::Size{logoWidth, logoHeight},
	        fontSize);

	logo = {boundary.topRight()
	            - Geom::Point{logoPad.right + logoWidth,
	                logoPad.bottom + logoHeight},
	    Geom::Size{logoWidth, logoHeight}};
}
}