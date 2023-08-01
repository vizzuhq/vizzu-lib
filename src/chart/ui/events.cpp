#include "events.h"

#include "base/conv/tostring.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::UI;

PointerEvent::PointerEvent(std::optional<int> pointerId,
    Geom::Point position,
    const Util::EventTarget *target) :
    Util::EventDispatcher::Params(target),
    position(position),
    pointerId(pointerId)
{}

std::string PointerEvent::dataToJson() const
{
	return "\"pointerId\":" + Conv::toString(pointerId)
	     + ",\"position\":" + position.toJSON();
}

WheelEvent::WheelEvent(double delta, const Util::EventTarget *target) :
    Util::EventDispatcher::Params(target),
    delta(delta)
{}

std::string WheelEvent::dataToJson() const
{
	return "\"delta\":" + Conv::toString(delta);
}
