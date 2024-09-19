
#include "colorbuilder.h"

#include <cstddef>
#include <cstdint>

#include "base/anim/interpolated.h"
#include "base/gfx/color.h"
#include "base/gfx/colorgradient.h"
#include "base/gfx/colorpalette.h"
#include "chart/generator/colorbase.h"

namespace Vizzu::Draw
{

ColorBuilder::ColorBuilder(const LightnessRange &lightnessRange,
    const Anim::Interpolated<Gfx::ColorPalette> &palette,
    const Gfx::ColorGradient &gradient) :
    lightnessRange(lightnessRange),
    gradient(gradient),
    palette(palette)
{}

Gfx::Color ColorBuilder::render(
    const Anim::Interpolated<Gen::ColorBase> &colorBase) const
{
	if (!colorBase.values[0].value.isDiscrete()
	    && !colorBase.get_or_first(::Anim::second)
	            .value.isDiscrete()) {
		auto pos = colorBase.combine(
		    [](const Gen::ColorBase &base)
		    {
			    return base.getPos();
		    });
		auto lightness = colorBase.combine(
		    [](const Gen::ColorBase &base)
		    {
			    return base.getLightness();
		    });
		return lightnessAdjusted(gradient(pos), lightness);
	}
	return colorBase.combine(
	    [this](const Gen::ColorBase &base)
	    {
		    return lightnessAdjusted(baseColor(base),
		        base.getLightness());
	    });
}

Gfx::Color ColorBuilder::render(const Gen::ColorBase &colorBase) const
{
	return lightnessAdjusted(baseColor(colorBase),
	    colorBase.getLightness());
}

Gfx::Color ColorBuilder::lightnessAdjusted(const Gfx::Color &color,
    double lightness) const
{
	auto factor = lightnessRange.scale(lightness);
	return color.lightnessScaled(factor);
}

Gfx::Color ColorBuilder::baseColor(
    const Gen::ColorBase &colorBase) const
{
	return colorBase.isDiscrete() ? indexedColor(colorBase.getIndex())
	                              : gradient(colorBase.getPos());
}

[[nodiscard]] Gfx::Color ColorBuilder::indexedColor(
    const uint32_t &colorIndex) const
{
	return palette.get().combine(
	    [&colorIndex](const Gfx::ColorPalette &palette)
	    {
		    return palette[static_cast<size_t>(colorIndex)];
	    });
}

}