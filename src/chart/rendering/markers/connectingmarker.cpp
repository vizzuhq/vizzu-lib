#include "connectingmarker.h"

#include <algorithm>
#include <cmath>

#include "base/anim/interpolated.h"
#include "base/geom/point.h"
#include "base/math/floating.h"
#include "base/math/fuzzybool.h"
#include "chart/generator/marker.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/options/channel.h"
#include "chart/options/coordsystem.h"
#include "chart/options/orientation.h"
#include "chart/options/shapetype.h"
#include "chart/rendering/drawingcontext.h"

#include "abstractmarker.h"

namespace Vizzu::Draw
{

ConnectingMarker::ConnectingMarker(const DrawingContext &ctx,
    const Gen::Marker &marker,
    ::Anim::InterpolateIndex lineIndex,
    Gen::ShapeType type) :
    AbstractMarker(marker, ctx.getOptions())
{
	using Math::Floating::less;
	auto isLine = type == Gen::ShapeType::line;
	auto isArea = type == Gen::ShapeType::area;

	auto needConnection =
	    !ctx.getOptions().geometry.interpolates()
	    || isConnecting(
	        ctx.getOptions().geometry.get_or_first(lineIndex).value);

	auto otherNeedConnection = isConnecting(
	    ctx.getOptions()
	        .geometry
	        .get_or_first(lineIndex == ::Anim::first ? ::Anim::second
	                                                 : ::Anim::first)
	        .value);

	auto polar = ctx.getOptions().coordSystem.factor<Math::FuzzyBool>(
	    Gen::CoordSystem::polar);
	auto horizontal =
	    ctx.getOptions().orientation.factor<Math::FuzzyBool>(
	        Gen::Orientation::horizontal);

	auto &&isHorizontal =
	    ctx.getOptions().orientation.get_or_first(lineIndex).value
	    == Gen::Orientation::horizontal;

	linear = !polar || horizontal
	      || Math::FuzzyBool::And(
	          !isHorizontal
	              && ctx.plot->axises.at(Gen::AxisId::x)
	                     .dimension.enabled,
	          ctx.plot->axises.at(Gen::AxisId::x)
	              .measure.enabled.factor(false),
	          !horizontal);

	lineWidth[0] = lineWidth[1] = 0;

	const auto *prev = getPrev(marker, lineIndex);

	labelEnabled =
	    ctx.getOptions().geometry.factor<Math::FuzzyBool>(type)
	    && marker.enabled;

	if (prev) {
		enabled = labelEnabled && prev->enabled;
		connected = Math::FuzzyBool::And(enabled,
		    marker.prevMainMarker.get_or_first(lineIndex).weight,
		    needConnection);
		if (auto &&pc =
		        marker.polarConnection.get_or_first(lineIndex);
		    pc.value) {
			auto &&newPolar =
			    Math::FuzzyBool::And(polar, pc.weight).more();
			linear = linear
			      || Math::FuzzyBool::Or(polar, pc.weight).more();
			connected = connected && newPolar && horizontal;
			enabled = enabled && newPolar && horizontal;

			if (polar != false && prev != &marker)
				linear = linear || Math::FuzzyBool{isHorizontal};
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
		    std::max(minWidth, maxWidth * marker.sizeFactor, less);

		auto horizontalFactor =
		    isArea ? static_cast<double>(horizontal.moreOrLess()) : 1;

		points[2] = pos;
		points[1] = pos
		          - (horizontal.more() != false
		                  ? marker.size.yComp() * horizontalFactor
		                  : marker.size.xComp() * horizontalFactor);

		if (prev) {
			auto prevSpacing = prev->spacing * prev->size / 2;
			auto prevPos = prev->position;

			if (polar != false && isHorizontal && prev != &marker) {
				if (!otherNeedConnection
				    && marker.polarConnection.get_or_first(lineIndex)
				           .value)
					prevPos.x = 0;
				else if (prevPos.x >= 1)
					prevPos.x -= 1;
			}

			points[3] = prevPos - prevSpacing;

			lineWidth[0] = isLine ? std::max(minWidth,
			                   maxWidth * prev->sizeFactor,
			                   less)
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

	setDataPosition(ctx.coordSys);
}

const Gen::Marker *ConnectingMarker::getPrev(
    const Gen::Marker &marker,
    ::Anim::InterpolateIndex lineIndex)
{
	const auto &prevId =
	    marker.prevMainMarker.get_or_first(lineIndex);
	return prevId.value.idx.empty() ? nullptr
	                                : &marker + prevId.value.distance;
}

}