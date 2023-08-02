#ifndef COLORBUILDER_H
#define COLORBUILDER_H

#include "base/gfx/color.h"
#include "base/gfx/colorgradient.h"
#include "base/gfx/colorpalette.h"

namespace Vizzu::Gen
{

struct ColorBuilder
{
	struct LighnessRange
	{
		double min;
		double max;
	};

	LighnessRange lighnessRange;
	const Gfx::ColorGradient *gradient;
	const Gfx::ColorPalette *palette;
	double color;
	double lightness;

	ColorBuilder();

	ColorBuilder(const LighnessRange &lighnessRange,
	    const Gfx::ColorPalette &palette,
	    int index,
	    double lightness);
	ColorBuilder(const LighnessRange &lighnessRange,
	    const Gfx::ColorGradient &gradient,
	    double pos,
	    double lightness);
	[[nodiscard]] Gfx::Color render() const;
	[[nodiscard]] Gfx::Color baseColor() const;
	[[nodiscard]] bool continuous() const;
};

}

#endif
