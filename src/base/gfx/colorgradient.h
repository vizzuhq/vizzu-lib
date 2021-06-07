#ifndef GFX_COLORGRADIENT
#define GFX_COLORGRADIENT

#include <vector>

#include "color.h"
#include "base/math/segmentedfunc.h"

namespace Gfx
{

struct ColorGradient : Math::SegmentedFunction<Gfx::Color>
{
	static ColorGradient HeatMap5Color();
	static ColorGradient HeatMap7Color();

	ColorGradient(Math::SegmentedFunction<Gfx::Color> gradient) :
	    Math::SegmentedFunction<Gfx::Color>(std::move(gradient))
	{}

	explicit ColorGradient(
	    const std::string &stoplist = std::string());

	explicit operator std::string() const;

	using Math::SegmentedFunction<Gfx::Color>::at;
};

}

#endif

