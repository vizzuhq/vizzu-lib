#include "drawchart.h"

#include "drawbackground.h"
#include "drawlabel.h"
#include "drawlegend.h"
#include "drawmarkerinfo.h"
#include "drawplot.h"
#include "logo.h"

namespace Vizzu::Draw
{
template <auto targetGetter, class MemberGetter>
void DrawChart::drawHeading(const MemberGetter &&getter)
{
	getter(getOptions())
	    .visit(
	        [&layout = getter(layout),
	            &style = getter(rootStyle),
	            &event = getter(rootEvents.draw),
	            this](int, const auto &weighted)
	        {
		        if (weighted.value.has_value()) {
			        DrawLabel(*this,
			            Geom::TransformedRect::fromRect(layout),
			            *weighted.value,
			            style,
			            event,
			            targetGetter(*weighted.value),
			            DrawLabel::Options(true,
			                std::max(weighted.weight * 2 - 1, 0.0)));
		        }
	        });
}

void DrawChart::draw()
{
	if (plot && rootEvents.draw.begin->invoke()) {

		DrawBackground(*this,
		    layout.boundary.outline(Geom::Size::Square(1)),
		    rootStyle,
		    rootEvents.draw.background,
		    Events::Targets::root());

		DrawPlot{*this};

		getOptions().legend.visit(
		    [this](int, const auto &legend)
		    {
			    if (legend.value)
				    DrawLegend(*this,
				        Gen::Options::toChannel(*legend.value),
				        legend.weight);
		    });

		drawHeading<&Events::Targets::chartTitle>(
		    [](auto &obj) -> decltype((obj.title))
		    {
			    return (obj.title);
		    });

		drawHeading<&Events::Targets::chartSubtitle>(
		    [](auto &obj) -> decltype((obj.subtitle))
		    {
			    return (obj.subtitle);
		    });

		drawHeading<&Events::Targets::chartCaption>(
		    [](auto &obj) -> decltype((obj.caption))
		    {
			    return (obj.caption);
		    });

		DrawMarkerInfo(layout, canvas, *plot);
	}

	if (auto logoElement = Events::Targets::logo();
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

	rootEvents.draw.complete->invoke();
}
}