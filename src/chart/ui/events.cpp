#include "events.h"

#include "base/conv/auto_json.h"
#include "base/conv/tostring.h"
#include "base/text/smartstring.h"

Vizzu::UI::PointerEvent::PointerEvent(std::optional<int> pointerId,
    Geom::Point position,
    const Util::EventTarget *target) :
    Util::EventDispatcher::Params(target),
    PointerEventDetail{position, pointerId}
{}

void Vizzu::UI::PointerEvent::appendToJSON(Conv::JSON &obj) const
{
	obj.any<PointerEventDetail>(*this);
}

Vizzu::UI::WheelEvent::WheelEvent(double delta,
    const Util::EventTarget *target) :
    Util::EventDispatcher::Params(target),
    WheelEventDetail{delta}
{}

void Vizzu::UI::WheelEvent::appendToJSON(Conv::JSON &obj) const
{
	obj.any<WheelEventDetail>(*this);
}
