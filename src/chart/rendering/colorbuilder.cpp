
#include "chart/rendering/colorbuilder.h"

#include "base/math/interpolation.h"

namespace Vizzu::Draw
{

ColorBuilder::ColorBuilder(const LighnessRange &lighnessRange,
    const Anim::Interpolated<Gfx::ColorPalette> &palette,
    const Gfx::ColorGradient &gradient) :
    lighnessRange(lighnessRange),
    gradient(gradient),
    palette(palette)
{}

Gfx::Color ColorBuilder::render(
    const Anim::Interpolated<Gen::ColorBase> &colorBase) const
{
	if (!colorBase.get(::Anim::first).value.isDiscrete()
	    && !colorBase.get(::Anim::secondIfExists)
	            .value.isDiscrete()) {
		auto pos = colorBase.combine<double>(
		    [](const Gen::ColorBase &base)
		    {
			    return base.getPos();
		    });
		auto lightness = colorBase.combine<double>(
		    [](const Gen::ColorBase &base)
		    {
			    return base.getLightness();
		    });
		return lightnessAdjusted(gradient.get().at(pos), lightness);
	}
	return colorBase.combine<Gfx::Color>(
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
	auto factor = lighnessRange.scale(lightness);
	return color.lightnessScaled(factor);
}

Gfx::Color ColorBuilder::baseColor(
    const Gen::ColorBase &colorBase) const
{
	return colorBase.isDiscrete()
	         ? indexedColor(colorBase.getIndex())
	         : gradient.get().at(colorBase.getPos());
}

[[nodiscard]] Gfx::Color ColorBuilder::indexedColor(
    const uint32_t &colorIndex) const
{
	return palette.get().combine<Gfx::Color>(
	    [&colorIndex](const Gfx::ColorPalette &palette)
	    {
		    return palette[static_cast<size_t>(colorIndex)];
	    });
}

}