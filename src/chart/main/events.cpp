#include "events.h"
#include "chart.h"

using namespace Vizzu;

Events::Events(Chart& chart) : chart(chart)
{
	auto& ed = chart.getEventDispatcher();
	update = ed.createEvent("update");
	draw.background = ed.createEvent("background-draw");
	draw.title = ed.createEvent("title-draw");
	draw.logo = ed.createEvent("logo-draw");
	draw.legend.background = ed.createEvent("legend-background-draw");
	draw.legend.title = ed.createEvent("legend-title-draw");
	draw.legend.label = ed.createEvent("legend-label-draw");
	draw.legend.marker = ed.createEvent("legend-marker-draw");
	draw.legend.bar = ed.createEvent("legend-bar-draw");
	draw.plot.background = ed.createEvent("plot-background-draw");
	draw.plot.marker.base = ed.createEvent("plot-marker-draw");
	draw.plot.marker.label = ed.createEvent("plot-marker-label-draw");
	draw.plot.marker.guide = ed.createEvent("plot-marker-guide-draw");
	draw.plot.axis.base = ed.createEvent("plot-axis-draw");
	draw.plot.axis.title = ed.createEvent("plot-axis-title-draw");
	draw.plot.axis.label = ed.createEvent("plot-axis-label-draw");
	draw.plot.axis.tick = ed.createEvent("plot-axis-tick-draw");
	draw.plot.axis.guide = ed.createEvent("plot-axis-guide-draw");
	draw.plot.axis.interlacing = ed.createEvent("plot-axis-interlacing-draw");
	animation.begin = ed.createEvent("animation-begin");
	animation.complete = ed.createEvent("animation-complete");
}