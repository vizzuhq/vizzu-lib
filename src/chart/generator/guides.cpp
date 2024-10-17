#include "guides.h"

#include "chart/options/align.h"
#include "chart/options/channel.h"
#include "chart/options/coordsystem.h"
#include "chart/options/options.h"
#include "chart/options/shapetype.h"

namespace Vizzu::Gen
{

bool GuidesByAxis::operator==(const GuidesByAxis &other) const
{
	return axis == other.axis && labels == other.labels
	    && axisGuides == other.axisGuides
	    && axisSticks == other.axisSticks
	    && markerGuides == other.markerGuides
	    && interlacings == other.interlacings;
}

Guides::Guides(const Options &options)
{
	const auto &channelX = options.getChannels().at(AxisId::x);
	const auto &channelY = options.getChannels().at(AxisId::y);
	if (channelX.isEmpty() && channelY.isEmpty()) return;

	auto xIsMeasure = channelX.isMeasure();
	auto yIsMeasure = channelY.isMeasure();
	auto isPolar = options.coordSystem.get() == CoordSystem::polar;
	auto isCircle = options.geometry.get() == ShapeType::circle;
	auto isHorizontal = options.isHorizontal();

	const auto &xOpt = options.getChannels().at(AxisId::x);
	const auto &yOpt = options.getChannels().at(AxisId::y);

	x.axis = xOpt.axisLine.getValue(yIsMeasure);
	y.axis = yOpt.axisLine.getValue(xIsMeasure && !isPolar);

	x.markerGuides = xOpt.markerGuides.getValue(
	    isCircle && yIsMeasure && !isPolar);

	y.markerGuides = yOpt.markerGuides.getValue(
	    isCircle && xIsMeasure && !isPolar);

	x.axisGuides = xOpt.guides.getValue(false);
	y.axisGuides = yOpt.guides.getValue(false);

	x.interlacings = xOpt.interlacing.getValue(
	    xIsMeasure && !isPolar && (!isHorizontal || !yIsMeasure));

	y.interlacings = yOpt.interlacing.getValue(
	    yIsMeasure && (isPolar || isHorizontal || !xIsMeasure));

	x.axisSticks =
	    xOpt.ticks.getValue((!isPolar || yIsMeasure) && xIsMeasure
	                        && yIsMeasure && isHorizontal);

	y.axisSticks = yOpt.ticks.getValue(
	    xIsMeasure && yIsMeasure && !isHorizontal);

	x.labels = xOpt.axisLabels.getValue(
	    (!isPolar || yIsMeasure)
	    && ((xIsMeasure && (x.axisSticks || x.interlacings))
	        || (!xIsMeasure && !xOpt.isEmpty())));

	auto stretchedPolar =
	    isPolar && !yIsMeasure
	    && (options.align == Base::Align::Type::stretch);

	y.labels = yOpt.axisLabels.getValue(
	    !stretchedPolar
	    && ((yIsMeasure && (y.axisSticks || y.interlacings))
	        || (!yIsMeasure && !yOpt.isEmpty())));
}

GuidesByAxis &Guides::at(AxisId channel)
{
	switch (channel) {
	default:
	case AxisId::x: return x;
	case AxisId::y: return y;
	}
}

const GuidesByAxis &Guides::at(AxisId channel) const
{
	switch (channel) {
	default:
	case AxisId::x: return x;
	case AxisId::y: return y;
	}
}

bool Guides::hasAnyGuides() const
{
	return y.markerGuides != false || x.markerGuides != false;
}

}