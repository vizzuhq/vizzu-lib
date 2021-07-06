#ifndef EVENTS_H
#define EVENTS_H

#include "base/io/log.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu
{

class Events {
public:
	Events(class Chart &chart);

	Util::EventDispatcher::event_ptr update;
	Util::EventDispatcher::event_ptr backgroundDraw;

protected:
    class Chart& chart;
};

}

#endif
