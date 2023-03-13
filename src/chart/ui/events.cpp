#include "events.h"

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "base/conv/tostring.h"

using namespace Vizzu;
using namespace Vizzu::UI;

MouseEvent::MouseEvent(Geom::Point position,
	const Diag::Marker *marker,
	Chart &chart) :
    Util::EventDispatcher::Params(&chart),
    marker(marker),
	position(position)
{
	elementUnder = chart.getLayout().getElementNameAt(position);
}

std::string MouseEvent::dataToJson() const
{
	std::string markerJson;
	auto coords = Geom::Point::Invalid();
	const auto &chart = *dynamic_cast<const Vizzu::Chart*>(sender);
	auto diagram = chart.getDiagram();
	if (diagram) {
		if (marker) markerJson = marker->toJson(diagram->getDataCube());
		coords = chart.getCoordSystem().getOriginal(position);
	}
	return
		"{"
			"\"element\":" + elementUnder
			+ "\"position\":" + std::string(position)
			+ ",\"coords\":" + std::string(coords)
			+ (!markerJson.empty() ? ", ": "")
			+ markerJson +
		"}";
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
