#include "events.h"

#include "base/io/log.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::UI;

ClickEvent::ClickEvent(Geom::Point position,
	const Diag::Marker *marker,
	Chart &chart) :
    Util::EventDispatcher::Params(&chart),
    marker(marker),
	position(position)
{
}

std::string ClickEvent::dataToJson() const
{
	std::string markerJson;
	if (marker) {
		const auto &chart = *dynamic_cast<const Vizzu::Chart*>(sender);
		auto diagram = chart.getDiagram();
		if (diagram)
			markerJson = marker->toJson(diagram->getDataCube());
	}
	return
		"{"
			"\"position\":" + std::string(position) + ", "
			+ markerJson +
		"}";
}

MouseOnMarkerEvent::MouseOnMarkerEvent(Chart &chart, const Diag::Marker *marker) :
    Util::EventDispatcher::Params(&chart), marker(marker)
{
}

std::string MouseOnMarkerEvent::dataToJson() const
{
	std::string markerJson;
	if (marker) {
		const auto &chart = *dynamic_cast<const Vizzu::Chart*>(sender);
		auto diagram = chart.getDiagram();
		if (diagram)
			markerJson = marker->toJson(diagram->getDataCube());
		return "{" + markerJson + "}";
	}
	return "\"\"";
}
