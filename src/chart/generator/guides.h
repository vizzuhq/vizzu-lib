#ifndef GUIDES_H
#define GUIDES_H

#include <stdexcept>

#include "base/math/interpolation.h"
#include "chart/options/options.h"

#include "axis.h"

namespace Vizzu::Gen
{

struct GuidesByAxis
{
	Math::FuzzyBool axis;
	Math::FuzzyBool labels;
	Math::FuzzyBool axisSticks;
	Math::FuzzyBool guidelines;
	Math::FuzzyBool interlacings;
	Math::FuzzyBool dimensionGuides;
	bool operator==(const GuidesByAxis &other) const;
};

GuidesByAxis interpolate(const GuidesByAxis &op0,
    const GuidesByAxis &op1,
    double factor);

struct Guides
{
	GuidesByAxis x;
	GuidesByAxis y;

	void init(const Axises &axises, const Options &options);
	[[nodiscard]] const GuidesByAxis &at(ChannelId channel) const;
	GuidesByAxis &at(ChannelId channel);
	[[nodiscard]] bool hasAnyGuides() const;
};

}

#endif
