#include "drawchart.h"

#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/geom/transformedrect.h"
#include "base/gfx/canvas.h"
#include "chart/main/layout.h"
#include "painter/painter.h"

#include "drawbackground.h"
#include "drawlabel.h"
#include "drawlegend.h"
#include "drawmarkerinfo.h"
#include "drawplot.h"
#include "logo.h"

namespace Vizzu::Draw
{
void DrawChart::drawBackground(Gfx::ICanvas &canvas,
    const Geom::Rect &bounds) const
{
	DrawBackground{{ctx()}}.draw(canvas,
	    bounds,
	    rootStyle,
	    *rootEvents.draw.background,
	    Events::Targets::root());
}

void DrawChart::drawPlot(Gfx::ICanvas &canvas,
    Painter &painter,
    const Geom::Rect &plotRect) const
{
	DrawPlot{{ctx()}}.draw(canvas, painter, plotRect);
}

void DrawChart::drawLegend(Gfx::ICanvas &canvas,
    const Geom::Rect &bounds) const
{
	auto &&legendObj = DrawLegend{{ctx()}};

	getOptions().legend.visit(
	    [&legendObj, &canvas, &bounds](::Anim::InterpolateIndex,
	        const auto &legend)
	    {
		    if (legend.value)
			    legendObj.draw(canvas,
			        bounds,
			        Gen::Options::toChannel(*legend.value),
			        legend.weight);
	    });
}

template <auto targetGetter, class MemberGetter>
void DrawChart::drawHeading(Gfx::ICanvas &canvas,
    const Layout &layout,
    const MemberGetter &&getter) const
{
	getter(getOptions())
	    .visit(
	        [&layout = getter(layout),
	            &style = getter(rootStyle),
	            &event = *getter(rootEvents.draw),
	            &canvas,
	            this](::Anim::InterpolateIndex, const auto &weighted)
	        {
		        if (weighted.value.has_value()) {
			        DrawLabel{{ctx()}}.draw(canvas,
			            Geom::TransformedRect::fromRect(layout),
			            *weighted.value,
			            style,
			            event,
			            targetGetter(*weighted.value),
			            {.colorTransform =
			                    Gfx::ColorTransform::None()
			                    * std::max(weighted.weight * 2 - 1,
			                        0.0)});
		        }
	        });
}

void DrawChart::drawMarkerInfo(Gfx::ICanvas &canvas,
    const Geom::Rect &bounds) const
{
	DrawMarkerInfo{{ctx()}, rootStyle.tooltip}.draw(canvas, bounds);
}

void DrawChart::drawLogo(Gfx::ICanvas &canvas,
    const Geom::Rect &bounds) const
{
	if (auto logoElement = Events::Targets::logo();
	    rootEvents.draw.logo->invoke(
	        Events::OnRectDrawEvent(*logoElement, {bounds, false}))) {

		Logo(canvas).draw(bounds.pos,
		    bounds.width(),
		    *rootStyle.logo.filter);

		renderedChart.emplace(Geom::TransformedRect::fromRect(bounds),
		    std::move(logoElement));
	}
}

void DrawChart::draw(Gfx::ICanvas &canvas, const Layout &layout) const
{
	Painter &painter = *static_cast<Painter *>(canvas.getPainter());
	painter.setCoordSys(coordSys);

	if (plot && rootEvents.draw.begin->invoke()) {

		drawBackground(canvas,
		    layout.boundary.outline(Geom::Size::Square(1)));

		drawPlot(canvas, painter, layout.plot);

		drawLegend(canvas, layout.legend);

		drawHeading<&Events::Targets::chartTitle>(canvas,
		    layout,
		    [](auto &obj) -> auto &
		    {
			    return obj.title;
		    });

		drawHeading<&Events::Targets::chartSubtitle>(canvas,
		    layout,
		    [](auto &obj) -> auto &
		    {
			    return obj.subtitle;
		    });

		drawHeading<&Events::Targets::chartCaption>(canvas,
		    layout,
		    [](auto &obj) -> auto &
		    {
			    return obj.caption;
		    });

		drawMarkerInfo(canvas, layout.boundary);
	}

	drawLogo(canvas, layout.logo);

	rootEvents.draw.complete->invoke();
}
}