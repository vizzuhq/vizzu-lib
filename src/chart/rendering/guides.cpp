#include "guides.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

void Guides::init(const Diag::Axises &axises,
    const Diag::Options &options)
{
	auto isCircle =
	    options.shapeType.get().getFactor(Diag::ShapeType::Circle);
	auto isLine =
	    options.shapeType.get().getFactor(Diag::ShapeType::Line);
	auto isHorizontal = options.horizontal.get();
	auto yIsContinous = axises.at(Diag::ScaleId::y).enabled;
	auto xIsContinous = axises.at(Diag::ScaleId::x).enabled;

	x.axis = yIsContinous;
	y.axis = xIsContinous;

	x.guidelines = isCircle && xIsContinous && !options.polar.get();
	y.guidelines = isCircle && yIsContinous && !options.polar.get();

	x.discreteGuides = isLine && xIsContinous;
	y.discreteGuides = isLine && yIsContinous;

	x.stripes =
	    yIsContinous
	    && (isHorizontal || (!isHorizontal && !xIsContinous));
	y.stripes =
	    xIsContinous
	    && (!isHorizontal || (isHorizontal && !yIsContinous));

	x.axisSticks =
	    xIsContinous && yIsContinous && !isHorizontal;
	y.axisSticks =
	    xIsContinous && yIsContinous && isHorizontal;
}

const GuidesByAxis &Guides::at(Diag::ScaleId scale) const
{
	if (scale == Diag::ScaleId::x) return x;
	if (scale == Diag::ScaleId::y) return y;
	throw std::out_of_range("guides index out of range");
}
