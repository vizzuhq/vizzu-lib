#include "events.h"
#include "chart.h"

using namespace Vizzu;

Events::Events(Chart& chart)
    : chart(chart)
{
}

void Events::createEvents() {
    auto& ed = chart.getEventDispatcher();
}