
#include "palettes.h"

using namespace Vizzu::Draw;

const Gfx::ColorPalette &Palettes::Default()
{
	typedef Gfx::Color C;
	static const Gfx::ColorPalette palette{
	    C::RGBA(100, 78, 180),
	    C::RGBA(4, 154, 100),
	    C::RGBA(252, 126, 4),
	    C::RGBA(244, 194, 4),
	    C::RGBA(212, 110, 36),
	    C::RGBA(212, 150, 100),
	    C::RGBA(244, 78, 76),
	    C::RGBA(188, 166, 4),
	    C::RGBA(132, 110, 28),
	    C::RGBA(252, 118, 60),
	    C::RGBA(180, 98, 172),
	    C::RGBA(244, 146, 252),
	    C::RGBA(188, 74, 148),
	    C::RGBA(156, 126, 244),
	    C::RGBA(156, 82, 180),
	    C::RGBA(108, 162, 252),
	    C::RGBA(92, 110, 188),
	    C::RGBA(124, 134, 140),
	    C::RGBA(172, 150, 140),
	    C::RGBA(76, 116, 80),
	    C::RGBA(172, 122, 76),
	    C::RGBA(124, 174, 84),
	    C::RGBA(76, 116, 80),
	    C::RGBA(156, 26, 108),
	    C::RGBA(172, 62, 148),
	    C::RGBA(180, 18, 4),
	};

	return palette;
}
