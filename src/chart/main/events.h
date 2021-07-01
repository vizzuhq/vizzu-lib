#ifndef EVENTS_H
#define EVENTS_H

#include "base/io/log.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu
{

class Events {
    friend class Chart;

public:
	Events(class Chart &chart);

	Util::EventDispatcher::event_ptr update;

protected:
    class Chart& chart;
};

}

#endif
