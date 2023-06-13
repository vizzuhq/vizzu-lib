
#include "chart/generator/colorbuilder.h"

#include "chart/main/style.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

ColorBuilder::ColorBuilder()
{
	lighnessRange = {0.4, -0.4};
	gradient = nullptr;
	palette = nullptr;
	color = 0;
	lightness = 0;
}

ColorBuilder::ColorBuilder(const LighnessRange &lighnessRange,
    const Gfx::ColorPalette &palette,
    int index,
    double lightness) :
    lighnessRange(lighnessRange),
    palette(&palette),
    lightness(lightness)
{
	gradient = nullptr;
	color = index;
}

ColorBuilder::ColorBuilder(const LighnessRange &lighnessRange,
    const Gfx::ColorGradient &gradient,
    double pos,
    double lightness) :
    lighnessRange(lighnessRange),
    gradient(&gradient),
    lightness(lightness)
{
	palette = nullptr;
	color = pos;
}

bool ColorBuilder::continuous() const { return gradient; }

Gfx::Color ColorBuilder::render() const
{
	auto factor = lighnessRange.min
	            + (lighnessRange.max - lighnessRange.min) * lightness;
	return baseColor().lightnessScaled(factor);
}

Gfx::Color ColorBuilder::baseColor() const
{
	if (gradient) { return gradient->at(color); }
	else if (palette) {
		return (*palette)[color];
	}
	else
		throw std::logic_error("no color palette or gradient set");
}
