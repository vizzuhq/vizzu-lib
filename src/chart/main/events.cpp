#include "events.h"
#include "chart.h"

using namespace Vizzu;

Events::Events(Chart& chart)
    : chart(chart)
{
	auto& ed = chart.getEventDispatcher();
	update = ed.createEvent("update");
	backgroundDraw = ed.createEvent("background-draw");
}