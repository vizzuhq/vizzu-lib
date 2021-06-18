#include "events.h"
#include "chart.h"

using namespace Vizzu;

Events::Events(Chart& chart)
    : chart(chart)
{
}

void Events::createEvents() {
    auto& ed = chart.getEventDispatcher();
    beginDraw = ed.createEvent("vizzu.chart.beginDraw");
    endDraw = ed.createEvent("vizzu.chart.endDraw");
    xyTest = ed.createEvent("vizzu.testEvents.xyParam");
}