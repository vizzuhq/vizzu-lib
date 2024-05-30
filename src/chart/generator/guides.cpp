#include "guides.h"

namespace Vizzu::Gen
{

GuidesByAxis interpolate(const GuidesByAxis &op0,
    const GuidesByAxis &op1,
    double factor)
{
	GuidesByAxis res;
	res.axis = interpolate(op0.axis, op1.axis, factor);
	res.labels = interpolate(op0.labels, op1.labels, factor);
	res.axisSticks =
	    interpolate(op0.axisSticks, op1.axisSticks, factor);
	res.axisGuides =
	    interpolate(op0.axisGuides, op1.axisGuides, factor);
	res.markerGuides =
	    interpolate(op0.markerGuides, op1.markerGuides, factor);
	res.interlacings =
	    interpolate(op0.interlacings, op1.interlacings, factor);
	return res;
}

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
	auto isCircle = options.geometry.get() == ShapeType::circle;
	auto isHorizontal = options.isHorizontal();
	const auto &channelX = options.getChannels().at(ChannelId::x);
	const auto &channelY = options.getChannels().at(ChannelId::y);
	auto xIsMeasure = channelX.isMeasure();
	auto yIsMeasure = channelY.isMeasure();
	auto isPolar = options.coordSystem.get() == CoordSystem::polar;

	const auto &xOpt = options.getChannels().at(ChannelId::x);
	const auto &yOpt = options.getChannels().at(ChannelId::y);

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

GuidesByAxis &Guides::at(ChannelId channel)
{
	if (channel == ChannelId::x) return x;
	if (channel == ChannelId::y) return y;
	throw std::out_of_range("guides index out of range");
}

const GuidesByAxis &Guides::at(ChannelId channel) const
{
	if (channel == ChannelId::x) return x;
	if (channel == ChannelId::y) return y;
	throw std::out_of_range("guides index out of range");
}

bool Guides::hasAnyGuides() const
{
	return y.markerGuides != false || x.markerGuides != false;
}

}