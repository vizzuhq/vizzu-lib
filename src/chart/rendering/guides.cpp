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
	auto yIsContinous = axises.at(Diag::Scale::Type::Y).enabled;
	auto xIsContinous = axises.at(Diag::Scale::Type::X).enabled;

	x.axis = yIsContinous;
	y.axis = xIsContinous;

	x.guidelines = isCircle && xIsContinous;
	y.guidelines = isCircle && yIsContinous;

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

const GuidesByAxis &Guides::at(Diag::Scale::Type scale) const
{
	if (scale == Diag::Scale::Type::X) return x;
	if (scale == Diag::Scale::Type::Y) return y;
	throw std::out_of_range("guides index out of range");
}
