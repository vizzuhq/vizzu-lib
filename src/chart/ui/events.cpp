#include "events.h"

#include "base/conv/auto_json.h"
#include "base/gui/pointer.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu::UI
{

PointerEvent::PointerEvent(const GUI::PointerEvent &event,
    const Util::EventTarget *target) :
    Util::EventDispatcher::Params(target),
    GUI::PointerEvent{event}
{}

void PointerEvent::appendToJSON(Conv::JSON &obj) const
{
	obj.any<GUI::PointerEvent>(*this);
}

WheelEvent::WheelEvent(double delta,
    const Util::EventTarget *target) :
    Util::EventDispatcher::Params(target),
    WheelEventDetail{delta}
{}

void WheelEvent::appendToJSON(Conv::JSON &obj) const
{
	obj.any<WheelEventDetail>(*this);
}

}