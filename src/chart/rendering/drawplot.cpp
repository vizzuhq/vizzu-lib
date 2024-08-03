#include "drawplot.h"

#include <utility>

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "chart/main/style.h"
#include "painter/painter.h"

#include "drawaxes.h"
#include "drawbackground.h"
#include "markerrenderer.h"
#include "renderedchart.h"

namespace Vizzu::Draw
{

void DrawPlot::draw(Gfx::ICanvas &canvas,
    Painter &painter,
    const Geom::Rect &plotRect) const
{
	DrawBackground{{ctx()}}.draw(canvas,
	    plotRect,
	    rootStyle.plot,
	    *rootEvents.draw.plot.background,
	    Events::Targets::plot());

	drawPlotArea(canvas, painter, false);

	auto axes = DrawAxes{{ctx()},
	    canvas,
	    painter,
	    {{ctx()}, canvas, painter}};
	axes.drawGeometries();

	auto clip = rootStyle.plot.overflow == Styles::Overflow::hidden;

	if (clip) {
		canvas.save();
		drawPlotArea(canvas, painter, true);
	}

	auto &&markerRenderer = MarkerRenderer::create(ctx());
	markerRenderer.drawLines(canvas, painter);

	markerRenderer.drawMarkers(canvas, painter);

	if (clip) canvas.restore();

	markerRenderer.drawLabels(canvas);

	axes.drawLabels();
}

void DrawPlot::drawPlotArea(Gfx::ICanvas &canvas,
    Painter &painter,
    bool clip) const
{
	auto areaElement = Events::Targets::area();

	auto rect = Geom::Rect::Ident();
	painter.setPolygonToCircleFactor(0.0);
	painter.setPolygonStraightFactor(0.0);

	if (clip) { painter.drawPolygon(rect.points(), true); }
	else {
		Events::OnRectDrawEvent eventObj(*areaElement, {rect, true});

		if (!rootStyle.plot.areaColor->isTransparent()) {
			canvas.setBrushColor(*rootStyle.plot.areaColor);
			canvas.setLineColor(*rootStyle.plot.areaColor);
			canvas.setLineWidth(0);
			if (rootEvents.draw.plot.area->invoke(
			        std::move(eventObj))) {
				painter.drawPolygon(rect.points(), false);
				renderedChart.emplace(Rect{rect, true},
				    std::move(areaElement));
			}
			canvas.setLineWidth(0);
		}
		else if (rootEvents.draw.plot.area->invoke(
		             std::move(eventObj))) {
			renderedChart.emplace(Rect{rect, true},
			    std::move(areaElement));
		}
	}
}

}