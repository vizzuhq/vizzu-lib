#ifndef CHART_UI_EVENTS_H
#define CHART_UI_EVENTS_H

#include <optional>

#include "base/geom/point.h"
#include "base/util/eventdispatcher.h"

namespace Vizzu::UI
{

struct PointerEventDetail
{
	Geom::Point position;
	std::optional<int> pointerId;
};

class PointerEvent :
    public Util::EventDispatcher::Params,
    public PointerEventDetail
{
public:
	PointerEvent(std::optional<int> pointerId,
	    Geom::Point position,
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
