#include "events.h"

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "base/conv/tostring.h"

using namespace Vizzu;
using namespace Vizzu::UI;

PointerEvent::PointerEvent(
	int pointerId,
	Geom::Point position,
	const Diag::Marker *marker,
	Chart &chart) :
    Util::EventDispatcher::Params(&chart),
    marker(marker),
    position(position),
    pointerId(pointerId)
{
	elementUnder = chart.getLayout().getElementNameAt(position);
}

std::string PointerEvent::dataToJson() const
{
	std::string markerJson;
	auto coords = Geom::Point::Invalid();
	const auto *chart = dynamic_cast<const Vizzu::Chart*>(sender);
	if (chart && chart->getDiagram()) {
		if (marker) markerJson = marker->toJson(chart->getDiagram()->getTable());
		coords = chart->getCoordSystem().getOriginal(position);
	}
	return
		"{"
			"\"element\":\"" + elementUnder + "\""
			+ ",\"pointerId\":" + std::to_string(pointerId)
			+ ",\"position\":" + std::string(position)
			+ ",\"coords\":" + std::string(coords)
			+ (!markerJson.empty() 
			  ? ",\"marker\":" + markerJson : std::string())
		+ "}";
}

WheelEvent::WheelEvent(double delta, Chart &chart) :
    Util::EventDispatcher::Params(&chart),
	delta(delta)
{
}

std::string WheelEvent::dataToJson() const
{
	return
		"{"
			"\"delta\":" + Conv::toString(delta) +
		"}";
}
