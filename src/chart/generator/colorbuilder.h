#ifndef COLORBUILDER_H
#define COLORBUILDER_H

#include "base/gfx/color.h"
#include "base/gfx/colorgradient.h"
#include "base/gfx/colorpalette.h"

namespace Vizzu
{
namespace Diag
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
	Gfx::Color render() const;
	Gfx::Color baseColor() const;
	bool continuous() const;
};

}
}

#endif
