#include "events.h"

#include "base/io/log.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::UI;

ClickEvent::ClickEvent(const Diag::Marker *marker, Chart &chart) :
    Util::EventDispatcher::Params(chart),
    marker(marker)
{
}

std::string ClickEvent::dataToJson() const
{
	if (marker)
	{
		auto &chart = dynamic_cast<Vizzu::Chart&>(sender);
		auto diagram = chart.getDiagram();
		if (diagram) {
			auto cell = diagram->getDataCube().getCellAsStrings(marker->index);
			auto list = Text::SmartString::map(cell, [](const auto &pair)
			{
				auto key = Text::SmartString::escape(pair.first, "\"\\");
				auto value = Text::SmartString::escape(pair.second, "\"\\");
				return "\"" + key + "\":\"" + value + "\"";
			});
			return "{\"marker\":{" + Text::SmartString::join(list, ",") + "}}";
		}
		else return "null";
	} else return "null";
}
