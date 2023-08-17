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

	LighnessRange lighnessRange{0.4, -0.4};
	const Gfx::ColorGradient *gradient{nullptr};
	const Gfx::ColorPalette *palette{nullptr};
	double color{0};
	double lightness{0};

	ColorBuilder() = default;

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
