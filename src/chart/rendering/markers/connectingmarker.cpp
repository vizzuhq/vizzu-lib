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

	const auto *prev =
	    getPrev(marker, ctx.plot->getMarkers(), lineIndex);

	labelEnabled =
	    ctx.getOptions().geometry.factor<Math::FuzzyBool>(type)
	    && marker.enabled;

	if (prev) {
		enabled = labelEnabled && prev->enabled;
		connected =
		    enabled
		    && Math::FuzzyBool{
		        marker.prevMainMarker.get_or_first(lineIndex).weight};
		if (auto &&pc =
		        marker.polarConnection.get_or_first(lineIndex);
		    pc.value) {
			auto &&newPolar =
			    (polar && Math::FuzzyBool{pc.weight}).more();
			linear = linear || polar.more()
			      || Math::FuzzyBool{pc.weight}.more();
			connected = connected && newPolar && horizontal;
			enabled = enabled && newPolar && horizontal;
		}
	}
	else {
		enabled = false;
		connected = false;
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

		if (prev) {
			auto prevSpacing = prev->spacing * prev->size / 2;
			auto prevPos = prev->position;

			if (polar != false
			    && ctx.getOptions()
			               .orientation.get_or_first(lineIndex)
			               .value
			           == Gen::Orientation::horizontal
			    && prev != &marker && prevPos.x >= 1) {
				prevPos.x -= 1;
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
	    marker.prevMainMarker.get_or_first(lineIndex);
	return prevId.value.idx != ~Gen::Marker::MarkerIndex{}
	         ? &markers[prevId.value.pos]
	         : nullptr;
}

}