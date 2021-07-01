#ifndef CHART_UI_EVENTS_H
#define CHART_UI_EVENTS_H

#include "base/util/eventdispatcher.h"

#include "chart/main/chart.h"
#include "chart/generator/marker.h"

namespace Vizzu
{
namespace UI
{

class ClickEvent : public Util::EventDispatcher::Params
{
public:
	ClickEvent(const Diag::Marker *marker, Chart &chart);
	std::string dataToJson() const;

private:
	const Diag::Marker *marker;
};

}
}

#endif
