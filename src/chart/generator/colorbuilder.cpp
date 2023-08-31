
#include "chart/generator/colorbuilder.h"

#include "chart/main/style.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

ColorBuilder::ColorBuilder(const LighnessRange &lighnessRange,
    const Gfx::ColorPalette &palette,
    int index,
    double lightness) :
    lighnessRange(lighnessRange),
    palette(&palette),
    color(index), lightness(lightness)
{


}

ColorBuilder::ColorBuilder(const LighnessRange &lighnessRange,
    const Gfx::ColorGradient &gradient,
    double pos,
    double lightness) :
    lighnessRange(lighnessRange),
    gradient(&gradient),
    color(pos), lightness(lightness)
{
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
	if (gradient) return gradient->at(color);
	if (palette) return (*palette)[static_cast<size_t>(color)];

	throw std::logic_error("no color palette or gradient set");
}
