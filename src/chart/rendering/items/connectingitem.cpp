#include "connectingitem.h"

#include <algorithm>

using namespace Vizzu;
using namespace Vizzu::Draw;

ConnectingItem::ConnectingItem(const Gen::Marker &marker,
    const CoordinateSystem &coordSys,
    const Gen::Options &options,
    const Styles::Chart &style,
    const Gen::Plot::Markers &markers,
    size_t lineIndex,
    Gen::ShapeType type) :
    DrawItem(marker, coordSys, options)
{
	auto isLine = type == Gen::ShapeType::line;
	auto isArea = type == Gen::ShapeType::area;

	linear = !options.polar || options.horizontal;

	lineWidth[0] = lineWidth[1] = 0;

	enabled = options.shapeType.factor<Math::FuzzyBool>(type);

	labelEnabled = enabled && marker.enabled;

	auto weight = marker.prevMainMarkerIdx.get(lineIndex).weight;

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
				linear = linear || options.polar.more();
				connected = connected && options.polar.more() && options.horizontal;
				enabled = enabled && options.polar && options.horizontal;
			}
			if (isArea) enabled = enabled && connected;
		}
		else
		{
			enabled = false;
			connected = false;
		}
	}

	auto spacing = marker.spacing * marker.size / 2;
	auto pos = marker.position - spacing;

	if (labelEnabled != false) {

		auto minWidth = isLine ? *style.plot.marker.lineMinWidth : 0;
		auto maxWidth = isLine ? *style.plot.marker.lineMaxWidth : 0;

		lineWidth[1] =
		    std::max(maxWidth * marker.sizeFactor, minWidth);

		auto horizontalFactor = isArea
			? fabs(2 * static_cast<double>(options.horizontal) - 1) : 1;

		points[2] = pos;
		points[1] = pos
		          - (options.horizontal.more() != false
		            ? marker.size.yComp() * horizontalFactor
		            : marker.size.xComp() * horizontalFactor);

		const auto *prev = getPrev(marker, markers, lineIndex);

		if (prev) {
			auto prevSpacing = prev->spacing * prev->size / 2;
			auto prevPos = prev->position;

			if (options.polar != false) {
				if (options.horizontal.more() != false) {
					if (prevPos.x >= 1) prevPos.x -= 1;
				}
			}

			points[3] = prevPos - prevSpacing;

			lineWidth[0] = isLine 
			    ? std::max(maxWidth * prev->sizeFactor, minWidth) : 0;

			points[0] = prevPos - prevSpacing
			          - (options.horizontal.more() != false
			            ? prev->size.yComp() * horizontalFactor
			            : prev->size.xComp() * horizontalFactor);

			center = isLine ? pos : Geom::Point(pos.x, 0);
		}
		else {
			center = points[3] = pos;
			points[0] = points[1];
			lineWidth[0] = lineWidth[1];
		}
	} else {
		center = Geom::Point(pos.x, 0);
	}

	radius = lineWidth[1] * coordSys.getRect().size.minSize();

	dataRect.pos = isLine ? points[2] : points[1];
	dataRect.size = points[2] - dataRect.pos;
}

const Gen::Marker *ConnectingItem::getPrev(
    const Gen::Marker &marker,
    const Gen::Plot::Markers &markers,
    size_t lineIndex)
{
	const auto &prevId = marker.prevMainMarkerIdx.get(lineIndex);
	return (prevId.weight > 0.0) ? &markers[prevId.value] : nullptr;
}
