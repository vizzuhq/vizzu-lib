#ifndef GUIDES_H
#define GUIDES_H

#include <stdexcept>

#include "chart/options/options.h"

#include "axis.h"

namespace Vizzu
{
namespace Diag
{

struct GuidesByAxis
{
	Math::FuzzyBool axis;
	Math::FuzzyBool axisSticks;
	Math::FuzzyBool guidelines;
	Math::FuzzyBool stripes;
	Math::FuzzyBool discreteGuides;
};

struct Guides
{
	GuidesByAxis x;
	GuidesByAxis y;

	void init(const Axises &axises, const Options &options);
	const GuidesByAxis &at(ScaleId scale) const;
	GuidesByAxis &at(ScaleId scale);
	bool hasAnyGuides() const;
};

}
}

#endif
