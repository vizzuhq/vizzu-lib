#ifndef CHART_UI_EVENTS_H
#define CHART_UI_EVENTS_H

#include <optional>

#include "base/geom/point.h"
#include "base/gui/pointer.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu::UI
{

class PointerEvent :
    public Util::EventDispatcher::Params,
    public GUI::PointerEvent
{
public:
	PointerEvent(const GUI::PointerEvent &event,
	    const Util::EventTarget *target);

	void appendToJSON(Conv::JSON &obj) const override;
};

struct WheelEventDetail
{
	double delta{};
};

class WheelEvent :
    public Util::EventDispatcher::Params,
    public WheelEventDetail
{
public:
	WheelEvent(double delta, const Util::EventTarget *target);

	void appendToJSON(Conv::JSON &obj) const override;
};

}

#endif
