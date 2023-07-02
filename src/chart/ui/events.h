#ifndef CHART_UI_EVENTS_H
#define CHART_UI_EVENTS_H

#include <optional>

#include "base/util/eventdispatcher.h"
#include "chart/generator/marker.h"
#include "chart/main/chart.h"

namespace Vizzu
{
namespace UI
{

class PointerEvent : public Util::EventDispatcher::Params
{
public:
	PointerEvent(std::optional<int> pointerId,
	    Geom::Point position,
	    const Util::EventTarget *target = nullptr);

	std::string dataToJson() const override;

	Geom::Point position;
	std::optional<int> pointerId;
};

class WheelEvent : public Util::EventDispatcher::Params
{
public:
	WheelEvent(double delta, Chart &chart);

	std::string dataToJson() const override;

	double delta;
};

}
}

#endif
