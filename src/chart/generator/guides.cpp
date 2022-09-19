#include "guides.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

void Guides::init(const Axises &axises,
    const Options &options)
{
	auto isCircle =
	    options.shapeType.get().getFactor(ShapeType::Circle);
	auto isLine =
	    options.shapeType.get().getFactor(ShapeType::Line);
	auto isHorizontal = options.horizontal.get();
	auto yIsContinous = axises.at(ScaleId::y).enabled;
	auto xIsContinous = axises.at(ScaleId::x).enabled;

	const auto &xOpt = options.getScales().at(ScaleId::x);
	const auto &yOpt = options.getScales().at(ScaleId::y);

	x.axis = xOpt.axisLine.get().getValue((bool)yIsContinous);
	y.axis = yOpt.axisLine.get().getValue((bool)xIsContinous);

	x.guidelines = isCircle && xIsContinous && !options.polar.get();
	y.guidelines = isCircle && yIsContinous && !options.polar.get();

	x.discreteGuides = xOpt.guides.get().getValue((bool)(isLine && xIsContinous));
	y.discreteGuides = yOpt.guides.get().getValue((bool)(isLine && yIsContinous));

	x.stripes = xOpt.interlacing.get().getValue((bool)(
	    xIsContinous
	    && (!isHorizontal || (isHorizontal && !yIsContinous))));

	y.stripes = yOpt.interlacing.get().getValue((bool)(
	    yIsContinous
	    && (isHorizontal || (!isHorizontal && !xIsContinous))));

	x.axisSticks = xOpt.ticks.get().getValue((bool)(
	    xIsContinous && yIsContinous && isHorizontal));

	y.axisSticks = yOpt.ticks.get().getValue((bool)(
	    xIsContinous && yIsContinous && !isHorizontal));
}

GuidesByAxis &Guides::at(ScaleId scale)
{
	if (scale == ScaleId::x) return x;
	if (scale == ScaleId::y) return y;
	throw std::out_of_range("guides index out of range");
}

const GuidesByAxis &Guides::at(ScaleId scale) const
{
	if (scale == ScaleId::x) return x;
	if (scale == ScaleId::y) return y;
	throw std::out_of_range("guides index out of range");
}

bool Guides::hasAnyGuides() const
{
	return (double)y.guidelines > 0 || (double)x.guidelines > 0;
}
