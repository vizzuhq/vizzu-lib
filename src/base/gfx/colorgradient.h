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

	explicit ColorGradient(
	    const std::string &stoplist = std::string());

	explicit operator std::string() const;
};

}

#endif
