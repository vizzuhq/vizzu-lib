#ifndef GFX_COLORGRADIENT
#define GFX_COLORGRADIENT

#include <vector>

#include "base/math/segmentedfunc.h"

#include "color.h"

namespace Gfx
{

struct ColorGradient : Math::SegmentedFunction<Color, ColorGradient>
{
	static ColorGradient HeatMap5Color();
	static ColorGradient HeatMap7Color();

	using SegmentedFunction::SegmentedFunction;

	[[nodiscard]] static ColorGradient fromString(
	    const std::string &stoplist);

	explicit operator std::string() const;

private:
	friend struct SegmentedFunction;
	ColorGradient() = default;
};

}

#endif
