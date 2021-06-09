#include "connectingitem.h"

#include <algorithm>

using namespace Vizzu;
using namespace Vizzu::Draw;

ConnectingDrawItem::ConnectingDrawItem(const Diag::Marker &item,
    const Diag::DiagramOptions &options,
    const Diag::Diagram::Markers &items,
    size_t lineIndex,
    Diag::ShapeType::Type type) :
    lineIndex(lineIndex)
{
	color = item.color;

	enabled = options.shapeType.get().getFactor(type);

	auto weight = item.prevMainItemIdx.values[lineIndex].weight;
	weight = std::max(0.0, 3 * weight - 2);

	connected = (double)enabled * weight * weight;

	if (weight > 0.0) {
		const auto *prev = getPrev(item, items, lineIndex);
		if (prev) {
			connected *= (double)(prev->enabled || item.enabled);
			if(prev->mainId.itemId > item.mainId.itemId) {
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
    const Diag::Marker &item,
    const Diag::Diagram::Markers &items,
    size_t lineIndex)
{
	const auto &prevId = item.prevMainItemIdx.values[lineIndex];
	return (prevId.weight > 0.0) ? &items[prevId.value] : nullptr;
}
