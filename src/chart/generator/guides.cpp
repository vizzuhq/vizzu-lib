#include "guides.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

GuidesByAxis Vizzu::Gen::interpolate(const GuidesByAxis &op0,
    const GuidesByAxis &op1,
    double factor)
{
	GuidesByAxis res;
	res.axis = interpolate(op0.axis, op1.axis, factor);
	res.labels = interpolate(op0.labels, op1.labels, factor);
	res.axisSticks =
	    interpolate(op0.axisSticks, op1.axisSticks, factor);
	res.dimensionGuides =
	    interpolate(op0.dimensionGuides, op1.dimensionGuides, factor);
	res.guidelines =
	    interpolate(op0.guidelines, op1.guidelines, factor);
	res.interlacings = interpolate(op0.interlacings, op1.interlacings, factor);
	return res;
}

bool GuidesByAxis::operator==(const GuidesByAxis &other) const
{
	return axis == other.axis && labels == other.labels
	    && dimensionGuides == other.dimensionGuides
	    && axisSticks == other.axisSticks
	    && guidelines == other.guidelines && interlacings == other.interlacings;
}

void Guides::init(const Axises &axises, const Options &options)
{
	auto isCircle =
	    options.shapeType.get().getFactor(ShapeType::Type::circle);
	auto isLine = options.shapeType.get().getFactor(ShapeType::Type::line);
	auto isHorizontal = options.horizontal.get();
	auto yIsMeasure =
	    axises.at(ChannelId::y).enabled.calculate<double>();
	auto xIsMeasure =
	    axises.at(ChannelId::x).enabled.calculate<double>();
	auto isPolar = options.polar.get();

	const auto &xOpt = options.getChannels().at(ChannelId::x);
	const auto &yOpt = options.getChannels().at(ChannelId::y);

	x.axis = xOpt.axisLine.get().getValue((bool)(yIsMeasure));
	y.axis = yOpt.axisLine.get().getValue(
	    (bool)(xIsMeasure && !isPolar));

	x.guidelines = xOpt.markerGuides.get().getValue(
	    (bool)(isCircle && yIsMeasure && !options.polar.get()));

	y.guidelines = yOpt.markerGuides.get().getValue(
	    (bool)(isCircle && xIsMeasure && !options.polar.get()));

	x.dimensionGuides =
	    xOpt.guides.get().getValue((bool)(isLine && xIsMeasure));
	y.dimensionGuides =
	    yOpt.guides.get().getValue((bool)(isLine && yIsMeasure));

	x.interlacings = xOpt.interlacing.get().getValue((
	    bool)(xIsMeasure && !isPolar
	          && (!isHorizontal || (isHorizontal && !yIsMeasure))));

	y.interlacings = yOpt.interlacing.get().getValue(
	    (bool)(yIsMeasure
	           && (isPolar || isHorizontal
	               || (!isHorizontal && !xIsMeasure))));

	x.axisSticks = xOpt.ticks.get().getValue(
	    (bool)(!(isPolar && !yIsMeasure) && xIsMeasure && yIsMeasure
	           && isHorizontal));

	y.axisSticks = yOpt.ticks.get().getValue(
	    (bool)(xIsMeasure && yIsMeasure && !isHorizontal));

	x.labels = xOpt.axisLabels.get().getValue(
	    (bool)(!(isPolar && !yIsMeasure)
	           && ((xIsMeasure && (x.axisSticks || x.interlacings))
	               || (!xIsMeasure && !xOpt.isEmpty()))));

	auto stretchedPolar =
	    isPolar && !yIsMeasure
	    && (options.alignType.get() == Base::Align::Type::stretch);

	y.labels = yOpt.axisLabels.get().getValue(
	    (bool)(!stretchedPolar
	           && ((yIsMeasure && (y.axisSticks || y.interlacings))
	               || (!yIsMeasure && !yOpt.isEmpty()))));
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
	return (double)y.guidelines > 0 || (double)x.guidelines > 0;
}
