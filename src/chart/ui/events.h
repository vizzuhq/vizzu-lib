#ifndef CHART_UI_EVENTS_H
#define CHART_UI_EVENTS_H

#include <optional>

#include "base/util/eventdispatcher.h"
#include "chart/generator/marker.h"
#include "chart/main/chart.h"

namespace Vizzu::UI
{

class PointerEvent : public Util::EventDispatcher::Params
{
public:
	PointerEvent(std::optional<int> pointerId,
	    Geom::Point position,
	    const Gen::Marker *marker,
	    Chart &chart);

	[[nodiscard]] std::string dataToJson() const override;

	std::string elementUnder;
	const Gen::Marker *marker;
	Geom::Point position;
	std::optional<int> pointerId;
};

class WheelEvent : public Util::EventDispatcher::Params
{
public:
	WheelEvent(double delta, Chart &chart);

	[[nodiscard]] std::string dataToJson() const override;

	double delta;
};

}

#endif
