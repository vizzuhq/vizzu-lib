#ifndef GUIDES_H
#define GUIDES_H

#include <stdexcept>

#include "chart/options/options.h"
#include "chart/generator/axis.h"

namespace Vizzu
{
namespace Draw
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

	void init(const Diag::Axises &axises, const Diag::Options &options);
	const GuidesByAxis &at(Diag::ScaleId scale) const;
};

}
}

#endif
