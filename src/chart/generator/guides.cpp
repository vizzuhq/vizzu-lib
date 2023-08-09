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
	res.interlacings =
	    interpolate(op0.interlacings, op1.interlacings, factor);
	return res;
}

bool GuidesByAxis::operator==(const GuidesByAxis &other) const
{
	return axis == other.axis && labels == other.labels
	    && dimensionGuides == other.dimensionGuides
	    && axisSticks == other.axisSticks
	    && guidelines == other.guidelines
	    && interlacings == other.interlacings;
}

void Guides::init(const MeasureAxises &measureAxises,
    const Options &options)
{
	auto isCircle = options.geometry.get() == ShapeType::circle;
	auto isLine = options.geometry.get() == ShapeType::line;
	auto isHorizontal = static_cast<bool>(options.horizontal);
	auto yIsMeasure =
	    static_cast<bool>(measureAxises.at(ChannelId::y)
	                          .enabled.calculate<Math::FuzzyBool>());
	auto xIsMeasure =
	    static_cast<bool>(measureAxises.at(ChannelId::x)
	                          .enabled.calculate<Math::FuzzyBool>());
	auto isPolar = options.coordSystem.get() == CoordSystem::polar;

	const auto &xOpt = options.getChannels().at(ChannelId::x);
	const auto &yOpt = options.getChannels().at(ChannelId::y);

	x.axis = xOpt.axisLine.getValue(yIsMeasure);
	y.axis = yOpt.axisLine.getValue(xIsMeasure && !isPolar);

	x.guidelines = xOpt.markerGuides.getValue(
	    isCircle && yIsMeasure && !isPolar);

	y.guidelines = yOpt.markerGuides.getValue(
	    isCircle && xIsMeasure && !isPolar);

	x.dimensionGuides = xOpt.guides.getValue(isLine && xIsMeasure);
	y.dimensionGuides = yOpt.guides.getValue(isLine && yIsMeasure);

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
	return y.guidelines != false || x.guidelines != false;
}
