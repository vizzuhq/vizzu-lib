#ifndef EVENTS_H
#define EVENTS_H

#include "base/io/log.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu
{

class Events {
    friend class Chart;

public:
    class XYParams : public Util::EventDispatcher::Params {
        public:
            XYParams(Util::EventDispatcher::Sender& sender, double x, double y)
                : Util::EventDispatcher::Params(sender), x(x), y(y) { }
            double x;
            double y;

            std::string toJsonString() override {
                return "{ " \
                    "\"event\": { \"name\": \"test.event\" }, " \
                    "\"sender\": { \"instance\": 0 }, " \
                    "\"x\": 3.14, \"y\": 6.28 }";
            }
            void fromJsonString(const char*) override {
            }
    };

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

    // vizzu.chart.beginDraw
    Util::EventDispatcher::event_ptr beginDraw;
    // vizzu.chart.endDraw
    Util::EventDispatcher::event_ptr endDraw;
    // vizzu.testEvents.xyParam
    Util::EventDispatcher::event_ptr xyTest;

    void createEvents();

protected:
    class Chart& chart;

    Events(class Chart& chart);
};

}

#endif //EVENTS_H
