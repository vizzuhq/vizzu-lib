#include "connectingmarker.h"

#include <algorithm>

#include "chart/generator/plot.h"

namespace Vizzu::Draw
{

ConnectingMarker::ConnectingMarker(const DrawingContext &ctx,
    const Gen::Marker &marker,
    ::Anim::InterpolateIndex lineIndex,
    Gen::ShapeType type) :
    AbstractMarker(marker, ctx.getOptions())
{
	auto isLine = type == Gen::ShapeType::line;
	auto isArea = type == Gen::ShapeType::area;

	auto polar = ctx.getOptions().coordSystem.factor<Math::FuzzyBool>(
	    Gen::CoordSystem::polar);
	auto horizontal =
	    ctx.getOptions().orientation.factor<Math::FuzzyBool>(
	        Gen::Orientation::horizontal);

	linear = !polar || horizontal;

	lineWidth[0] = lineWidth[1] = 0;

	enabled = ctx.getOptions().geometry.factor<Math::FuzzyBool>(type);

	labelEnabled = enabled && marker.enabled;

	auto weight =
	    marker.prevMainMarkerIdx.get_or_first(lineIndex).weight;

	connected = enabled && Math::FuzzyBool(weight);

	if (weight > 0.0) {
		const auto *prev =
		    getPrev(marker, ctx.plot->getMarkers(), lineIndex);
		if (prev) {
			labelEnabled =
			    enabled && (marker.enabled || prev->enabled);
			connected =
			    connected && (prev->enabled || marker.enabled);
			if (prev->mainId.get_or_first(lineIndex).value.itemId
			    > marker.mainId.get_or_first(lineIndex)
			          .value.itemId) {
				linear = linear || polar.more();
				connected = connected && polar.more() && horizontal;
				enabled = enabled && polar && horizontal;
			}
			if (isArea) enabled = enabled && connected;
		}
		else {
			enabled = false;
			connected = false;
		}
	}

	auto spacing = marker.spacing * marker.size / 2;
	auto pos = marker.position - spacing;

	if (labelEnabled != false) {

		auto minWidth =
		    isLine ? *ctx.rootStyle.plot.marker.lineMinWidth : 0;
		auto maxWidth =
		    isLine ? *ctx.rootStyle.plot.marker.lineMaxWidth : 0;

		lineWidth[1] =
		    std::max(minWidth, maxWidth * marker.sizeFactor);

		auto horizontalFactor =
		    isArea ? fabs(2 * static_cast<double>(horizontal) - 1)
		           : 1;

		points[2] = pos;
		points[1] = pos
		          - (horizontal.more() != false
		                  ? marker.size.yComp() * horizontalFactor
		                  : marker.size.xComp() * horizontalFactor);

		const auto *prev =
		    getPrev(marker, ctx.plot->getMarkers(), lineIndex);

		if (prev) {
			auto prevSpacing = prev->spacing * prev->size / 2;
			auto prevPos = prev->position;

			if (polar != false) {
				if (horizontal.more() != false) {
					if (prevPos.x >= 1) prevPos.x -= 1;
				}
			}

			points[3] = prevPos - prevSpacing;

			lineWidth[0] = isLine ? std::max(minWidth,
			                   maxWidth * prev->sizeFactor)
			                      : 0;

			points[0] =
			    prevPos - prevSpacing
			    - (horizontal.more() != false
			            ? prev->size.yComp() * horizontalFactor
			            : prev->size.xComp() * horizontalFactor);

			center = isLine ? pos : Geom::Point{pos.x, 0};
		}
		else {
			center = points[3] = pos;
			points[0] = points[1];
			lineWidth[0] = lineWidth[1];
		}
	}
	else {
		center = Geom::Point{pos.x, 0};
	}

	radius = lineWidth[1] * ctx.coordSys.getRect().size.minSize();

	dataRect.pos = isLine ? points[2] : points[1];
	dataRect.size = Geom::Size{points[2] - dataRect.pos};
}

const Gen::Marker *ConnectingMarker::getPrev(
    const Gen::Marker &marker,
    const Gen::Plot::Markers &markers,
    ::Anim::InterpolateIndex lineIndex)
{
	const auto &prevId =
	    marker.prevMainMarkerIdx.get_or_first(lineIndex);
	return (prevId.weight > 0.0) ? &markers[prevId.value] : nullptr;
}

}