#include "connectingitem.h"

#include <algorithm>

using namespace Vizzu;
using namespace Vizzu::Draw;

ConnectingDrawItem::ConnectingDrawItem(const Gen::Marker &marker,
    const CoordinateSystem &coordSys,
    const Gen::Options &options,
    const Gen::Plot::Markers &markers,
    size_t lineIndex,
    Gen::ShapeType type) :
    DrawItem(marker, coordSys, options)
{
	linear = false;
	color = marker.color;

	enabled = options.shapeType.factor<Math::FuzzyBool>(type);
	labelEnabled = enabled && marker.enabled;

	auto weight = marker.prevMainMarkerIdx.values[lineIndex].weight;
//	weight = std::max(0.0, 1.5 * weight - 0.5);

	connected = enabled && Math::FuzzyBool(weight);

	if (weight > 0.0) {
		const auto *prev = getPrev(marker, markers, lineIndex);
		if (prev) {
			labelEnabled =
			    enabled && (marker.enabled || prev->enabled);
			connected =
			    connected && (prev->enabled || marker.enabled);
			if (prev->mainId.get(lineIndex).value.itemId 
				> marker.mainId.get(lineIndex).value.itemId) 
			{
				linear = options.polar.more();
				connected = connected && options.polar.more() && options.horizontal;
				enabled = enabled && options.polar && options.horizontal;
			}
		}
		else
			connected = 0;
	}
}

const Gen::Marker *ConnectingDrawItem::getPrev(
    const Gen::Marker &marker,
    const Gen::Plot::Markers &markers,
    size_t lineIndex)
{
	const auto &prevId = marker.prevMainMarkerIdx.values[lineIndex];
	return (prevId.weight > 0.0) ? &markers[prevId.value] : nullptr;
}
