#include "drawchart.h"

#include "drawbackground.h"
#include "drawlabel.h"
#include "drawlegend.h"
#include "drawmarkerinfo.h"
#include "drawplot.h"
#include "logo.h"

namespace Vizzu::Draw
{
template <class T>
void DrawChart::drawHeading(const Gen::Options::Heading &option,
    const Styles::Label &style,
    const Geom::Rect &layout,
    const Util::EventDispatcher::event_ptr &event)
{
	option.visit(
	    [&, this](int, const auto &weighted)
	    {
		    if (weighted.value.has_value()) {
			    DrawLabel(*this,
			        Geom::TransformedRect::fromRect(layout),
			        *weighted.value,
			        style,
			        event,
			        std::make_unique<T>(*weighted.value),
			        DrawLabel::Options(true,
			            std::max(weighted.weight * 2 - 1, 0.0)));
		    }
	    });
}

void DrawChart::draw()
{
	if (plot
	    && (!rootEvents.draw.begin
	        || rootEvents.draw.begin->invoke(
	            Util::EventDispatcher::Params{}))) {

		DrawBackground(*this,
		    layout.boundary.outline(Geom::Size::Square(1)),
		    plot->getStyle(),
		    rootEvents.draw.background,
		    std::make_unique<Events::Targets::Root>());

		DrawPlot{*this};

		plot->getOptions()->legend.visit(
		    [&, this](int, const auto &legend)
		    {
			    if (legend.value)
				    DrawLegend(*this,
				        Gen::Options::toChannel(*legend.value),
				        legend.weight);
		    });

		drawHeading<Events::Targets::ChartTitle>(
		    plot->getOptions()->title,
		    plot->getStyle().title,
		    layout.title,
		    rootEvents.draw.title);

		drawHeading<Events::Targets::ChartSubtitle>(
		    plot->getOptions()->subtitle,
		    plot->getStyle().subtitle,
		    layout.subtitle,
		    rootEvents.draw.subtitle);

		drawHeading<Events::Targets::ChartCaption>(
		    plot->getOptions()->caption,
		    plot->getStyle().caption,
		    layout.caption,
		    rootEvents.draw.caption);

		DrawMarkerInfo(layout, canvas, *plot);
	}

	if (auto logoElement = std::make_unique<Events::Targets::Logo>();
	    rootEvents.draw.logo->invoke(
	        Events::OnRectDrawEvent(*logoElement,
	            {layout.logo, false}))) {

		Logo(canvas).draw(layout.logo.pos,
		    layout.logo.width(),
		    *rootStyle.logo.filter);

		renderedChart.emplace(
		    Geom::TransformedRect::fromRect(layout.logo),
		    std::move(logoElement));
	}

	if (rootEvents.draw.complete)
		rootEvents.draw.complete->invoke(
		    Util::EventDispatcher::Params{});
}
}