#include "events.h"

#include "chart.h"

using namespace Vizzu;

Events::Targets::Targets() :
	root("root", nullptr),
	title("title", &root),
	legend("legend", &root),
	legendTitle("legend.title", &legend),
	legendLabel("legend.label", &legend),
	legendMarker("legend.marker", &legend),
	legendBar("legend.bar", &legend),
	plot("plot", &root),
	area("plot.area", &plot),
	xAxis("plot.xAxis", &plot),
	yAxis("plot.yAxis", &plot),
	xTitle("plot.xAxis.title", &xAxis),
	yTitle("plot.yAxis.title", &yAxis),
	xLabel("plot.xAxis.label", &xAxis),
	yLabel("plot.yAxis.label", &yAxis),
	xInterlacing("plot.xAxis.interlacing", &xAxis),
	yInterlacing("plot.yAxis.interlacing", &yAxis),
	xGuide("plot.xAxis.guide", &xAxis),
	yGuide("plot.yAxis.guide", &yAxis),
	xTick("plot.xAxis.tick", &xAxis),
	yTick("plot.yAxis.tick", &yAxis),
	marker("plot.marker", &area),
	markerLabel("plot.marker.label", &marker),
	markerXGuide("plot.marker.xGuide", &marker),
	markerYGuide("plot.marker.yGuide", &marker),
	logo("logo", &root)
{}

Events::Events(Chart &chart) : chart(chart)
{
	auto &ed = chart.getEventDispatcher();
	draw.begin = ed.createEvent("draw-begin");
	draw.complete = ed.createEvent("draw-complete");
	draw.background = ed.createEvent("background-draw");
	draw.title = ed.createEvent("title-draw");
	draw.logo = ed.createEvent("logo-draw");
	draw.legend.background = ed.createEvent("legend-background-draw");
	draw.legend.title = ed.createEvent("legend-title-draw");
	draw.legend.label = ed.createEvent("legend-label-draw");
	draw.legend.marker = ed.createEvent("legend-marker-draw");
	draw.legend.bar = ed.createEvent("legend-bar-draw");
	draw.plot.background = ed.createEvent("plot-background-draw");
	draw.plot.area = ed.createEvent("plot-area-draw");
	draw.plot.marker.base = ed.createEvent("plot-marker-draw");
	draw.plot.marker.label = ed.createEvent("plot-marker-label-draw");
	draw.plot.marker.guide = ed.createEvent("plot-marker-guide-draw");
	draw.plot.axis.base = ed.createEvent("plot-axis-draw");
	draw.plot.axis.title = ed.createEvent("plot-axis-title-draw");
	draw.plot.axis.label = ed.createEvent("plot-axis-label-draw");
	draw.plot.axis.tick = ed.createEvent("plot-axis-tick-draw");
	draw.plot.axis.guide = ed.createEvent("plot-axis-guide-draw");
	draw.plot.axis.interlacing =
	    ed.createEvent("plot-axis-interlacing-draw");
	animation.begin = ed.createEvent("animation-begin");
	animation.update = ed.createEvent("update");
	animation.complete = ed.createEvent("animation-complete");
}