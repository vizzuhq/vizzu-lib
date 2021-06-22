#ifndef EVENTS_H
#define EVENTS_H

#include "base/io/log.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu
{

class Events {
    friend class Chart;

public:
    Util::EventDispatcher::Params invoke(const Util::EventDispatcher::event_ptr& event) const {
        auto sender = (Util::EventDispatcher::Sender&)(chart);
        Util::EventDispatcher::Params params(sender);
        event->invoke(params);
        return params;
    }

    template<typename P, typename ... Args>
    P invoke(const Util::EventDispatcher::event_ptr& event, Args... args) const {
        auto sender = (Util::EventDispatcher::Sender&)(chart);
        P params(sender, args...);
        event->invoke(params);
        return params;
    }

    Util::EventDispatcher::event_ptr update;

    void createEvents();

protected:
    class Chart& chart;

    Events(class Chart& chart);
};

}

#endif //EVENTS_H
