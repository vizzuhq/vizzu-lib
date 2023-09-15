
#include "chart/rendering/colorbuilder.h"

#include "base/math/interpolation.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

ColorBuilder::ColorBuilder(const LighnessRange &lighnessRange,
    const Anim::Interpolated<Gfx::ColorPalette> &palette,
    const Gfx::ColorGradient &gradient) :
    lighnessRange(lighnessRange),
    gradient(&gradient),
    palette(&palette)
{}

Gfx::Color ColorBuilder::render(
    const Anim::Interpolated<Gen::ColorBase> &colorBase) const
{
	if (!colorBase.get(0).value.discrete
	    && !colorBase.get(1).value.discrete) {
		auto pos = colorBase.combine<double>(
		    [&](int, const Gen::ColorBase &base)
		    {
			    return base.pos;
		    });
		auto lightness = colorBase.combine<double>(
		    [&](int, const Gen::ColorBase &base)
		    {
			    return base.lightness;
		    });
		return lightnessAdjusted(gradient->at(pos), lightness);
	}
	return colorBase.combine<Gfx::Color>(
	    [&](int, const Gen::ColorBase &base)
	    {
		    return lightnessAdjusted(baseColor(base), base.lightness);
	    });
}

Gfx::Color ColorBuilder::render(const Gen::ColorBase &colorBase) const
{
	return lightnessAdjusted(baseColor(colorBase),
	    colorBase.lightness);
}

Gfx::Color ColorBuilder::lightnessAdjusted(const Gfx::Color &color,
    double lightness) const
{
	auto factor = lighnessRange.scale(lightness);
	return color.lightnessScaled(factor);
}

Gfx::Color ColorBuilder::baseColor(
    const Gen::ColorBase &colorBase) const
{
	return colorBase.discrete ? indexedColor(colorBase.index)
	                          : gradient->at(colorBase.pos);
}

[[nodiscard]] Gfx::Color ColorBuilder::indexedColor(
    const uint32_t &colorIndex) const
{
	return palette->combine<Gfx::Color>(
	    [&](int, const Gfx::ColorPalette &palette)
	    {
		    return palette[static_cast<size_t>(colorIndex)];
	    });
}
