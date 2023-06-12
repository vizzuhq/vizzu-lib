#ifndef CHART_UI_EVENTS_H
#define CHART_UI_EVENTS_H

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
	PointerEvent(int pointerId,
	    Geom::Point position,
	    const Diag::Marker *marker,
	    Chart &chart);

	std::string dataToJson() const override;

	std::string elementUnder;
	const Diag::Marker *marker;
	Geom::Point position;
	int pointerId;
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
