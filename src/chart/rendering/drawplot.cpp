#include "drawplot.h"

#include "base/text/smartstring.h"
#include "chart/rendering/drawaxes.h"
#include "chart/rendering/drawbackground.h"

#include "markerrenderer.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Gen;

DrawPlot::DrawPlot(const DrawingContext &context) :
    DrawingContext(context)
{
	auto plotElement = std::make_unique<Events::Targets::Plot>();

	DrawBackground(*this,
	    layout.plot,
	    rootStyle.plot,
	    rootEvents.draw.plot.background,
	    std::move(plotElement));

	drawArea(false);
	DrawAxes(*this).drawBase();

	auto clip = rootStyle.plot.overflow == Styles::Overflow::hidden;

	if (clip) clipPlotArea();

	drawMarkerGuides();

	drawMarkers();

	if (clip) canvas.restore();

	drawMarkerLabels();

	DrawAxes(*this).drawLabels();
}

void DrawPlot::clipPlotArea()
{
	canvas.save();
	drawArea(true);
}

void DrawPlot::drawArea(bool clip)
{
	auto areaElement = std::make_unique<Events::Targets::Area>();

	Geom::Rect rect(Geom::Point(), Geom::Size::Identity());
	painter.setPolygonToCircleFactor(0.0);
	painter.setPolygonStraightFactor(0.0);
	painter.setResMode(ResolutionMode::High);

	if (clip) { painter.drawPolygon(rect.points(), true); }
	else {
		Events::OnRectDrawParam eventObj(*areaElement, rect);

		if (!rootStyle.plot.areaColor->isTransparent()) {
			canvas.setBrushColor(*rootStyle.plot.areaColor);
			canvas.setLineColor(*rootStyle.plot.areaColor);
			canvas.setLineWidth(0);
			if (!rootEvents.draw.plot.area
			    || rootEvents.draw.plot.area->invoke(std::move(eventObj))) 
			{
				painter.drawPolygon(rect.points(), false);
				renderedChart->emplace(Rect{ rect, true },
					std::move(areaElement));
			}
			canvas.setLineWidth(0);
		}
		else if (rootEvents.draw.plot.area)
			rootEvents.draw.plot.area->invoke(std::move(eventObj));
	}
}

void DrawPlot::drawMarkerGuides()
{
	const auto &style = plot.getStyle().plot.marker.guides;

	if (!style.color->isTransparent() && *style.lineWidth > 0
	    && static_cast<double>(plot.anyAxisSet) > 0
	    && plot.guides.hasAnyGuides()) {
		canvas.setLineWidth(*style.lineWidth);

		auto origo = plot.axises.origo();

		for (const auto &marker : plot.getMarkers())
			MarkerRenderer(marker, *this).drawLines(style, origo);

		canvas.setLineWidth(0);
	}
}

void DrawPlot::drawMarkers()
{
	for (const auto &marker : plot.getMarkers())
		MarkerRenderer(marker, *this).draw();
}

void DrawPlot::drawMarkerLabels()
{
	for (const auto &marker : plot.getMarkers())
		MarkerRenderer(marker, *this).drawLabel();
}
