#ifndef CHART_UI_EVENTS_H
#define CHART_UI_EVENTS_H

#include <optional>

#include "base/geom/point.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu::UI
{

class PointerEvent : public Util::EventDispatcher::Params
{
public:
	PointerEvent(std::optional<int> pointerId,
	    Geom::Point position,
	    const Util::EventTarget *target);

	[[nodiscard]] std::string dataToJson() const override;

	Geom::Point position;
	std::optional<int> pointerId;
};

class WheelEvent : public Util::EventDispatcher::Params
{
public:
	WheelEvent(double delta, const Util::EventTarget *target);

	[[nodiscard]] std::string dataToJson() const override;

	double delta;
};

}

#endif
