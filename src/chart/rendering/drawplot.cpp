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
	DrawBackground(layout.plot,
	    canvas,
	    rootStyle.plot,
	    rootEvents.plot.background,
	    Events::OnRectDrawParam("plot"));

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
	std::array<Geom::Point, 4> points = {Geom::Point(0.0, 0.0),
	    Geom::Point(0.0, 1.0),
	    Geom::Point(1.0, 1.0),
	    Geom::Point(1.0, 0.0)};
	painter.setPolygonToCircleFactor(0.0);
	painter.setPolygonStraightFactor(0.0);
	painter.setResMode(ResolutionMode::High);

	if (clip) { painter.drawPolygon(points, true); }
	else {
		auto boundary = Geom::Rect::Boundary(points);
		auto p0 = coordSys.convert(boundary.bottomLeft());
		auto p1 = coordSys.convert(boundary.topRight());
		auto rect = Geom::Rect(p0, p1 - p0).positive();

		Events::OnRectDrawParam eventObj("plot.area", rect);

		if (!rootStyle.plot.areaColor->isTransparent()) {
			canvas.setBrushColor(*rootStyle.plot.areaColor);
			canvas.setLineColor(*rootStyle.plot.areaColor);
			canvas.setLineWidth(0);
			if (!rootEvents.plot.area
			    || rootEvents.plot.area->invoke(std::move(eventObj))) {
				painter.drawPolygon(points, false);
			}
			canvas.setLineWidth(0);
		}
		else if (rootEvents.plot.area)
			rootEvents.plot.area->invoke(std::move(eventObj));
	}
}

void DrawPlot::drawMarkerGuides()
{
	auto &style = plot.getStyle().plot.marker.guides;

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
