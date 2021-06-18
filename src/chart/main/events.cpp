#include "events.h"
#include "chart.h"

using namespace Vizzu;

Events::Events() {
}

void Events::createEvents(std::shared_ptr<class Chart> chart) {
    this->chart = chart;
    auto& ed = chart->getEventDispatcher();
    beginDraw = ed.createEvent("vizzu.chart.beginDraw");
    endDraw = ed.createEvent("vizzu.chart.endDraw");
    xyTest = ed.createEvent("vizzu.testEvents.xyParam");
}