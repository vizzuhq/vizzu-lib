#ifndef CHART_UI_EVENTS_H
#define CHART_UI_EVENTS_H

#include "base/util/eventdispatcher.h"

#include "chart/main/chart.h"
#include "chart/generator/marker.h"

namespace Vizzu
{
namespace UI
{

class MouseEvent : public Util::EventDispatcher::Params
{
public:
	MouseEvent(Geom::Point position,
		const Diag::Marker *marker,
		Chart &chart);

	std::string dataToJson() const override;

	const Diag::Marker *marker;
	Geom::Point position;
};

}
}

#endif
