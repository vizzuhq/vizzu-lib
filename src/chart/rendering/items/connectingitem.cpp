#include "connectingitem.h"

#include <algorithm>

using namespace Vizzu;
using namespace Vizzu::Draw;

ConnectingDrawItem::ConnectingDrawItem(const Diag::Marker &marker,
    const Diag::DiagramOptions &options,
    const Diag::Diagram::Markers &markers,
    size_t lineIndex,
    Diag::ShapeType::Type type) :
    lineIndex(lineIndex)
{
	color = marker.color;

	enabled = options.shapeType.get().getFactor(type);

	auto weight = marker.prevMainMarkerIdx.values[lineIndex].weight;
	weight = std::max(0.0, 3 * weight - 2);

	connected = (double)enabled * weight * weight;

	if (weight > 0.0) {
		const auto *prev = getPrev(marker, markers, lineIndex);
		if (prev) {
			connected *= (double)(prev->enabled || marker.enabled);
			if(prev->mainId.itemId > marker.mainId.itemId) {
				auto secondHalf =
					std::max(0.0, 2 * (double)options.polar.get() - 1);
				connected *= secondHalf;
				enabled = enabled * (double)options.polar.get();
			}
		}
		else connected = 0;
	}
}

const Diag::Marker *ConnectingDrawItem::getPrev(
    const Diag::Marker &marker,
    const Diag::Diagram::Markers &markers,
    size_t lineIndex)
{
	const auto &prevId = marker.prevMainMarkerIdx.values[lineIndex];
	return (prevId.weight > 0.0) ? &markers[prevId.value] : nullptr;
}
