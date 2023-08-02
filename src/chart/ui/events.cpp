#include "events.h"

#include "base/conv/tostring.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::UI;

PointerEvent::PointerEvent(std::optional<int> pointerId,
    Geom::Point position,
    const Gen::Marker *marker,
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
	const auto *chart = static_cast<const Vizzu::Chart *>(target);
	if (chart && chart->getPlot()) {
		if (marker)
			markerJson =
			    marker->toJson(chart->getPlot()->getTable());
		coords = chart->getCoordSystem().getOriginal(position);
	}
	return R"("element":")" + elementUnder + "\""
	     + ",\"pointerId\":" + Conv::toString(pointerId)
	     + ",\"position\":" + position.toJSON()
	     + ",\"coords\":" + coords.toJSON()
	     + (!markerJson.empty() ? ",\"marker\":" + markerJson
	                            : std::string());
}

WheelEvent::WheelEvent(double delta, Chart &chart) :
    Util::EventDispatcher::Params(&chart),
    delta(delta)
{}

std::string WheelEvent::dataToJson() const
{
	return "\"delta\":" + Conv::toString(delta);
}
