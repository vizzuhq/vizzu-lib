#ifndef GFX_LINEARGRADIENT_H
#define GFX_LINEARGRADIENT_H

#include "base/geom/line.h"

#include "colorgradient.h"

namespace Gfx
{

struct LinearGradient
{
	Geom::Line line;
	ColorGradient colors;
};

}

#endif // GFX_LINEARGRADIENT_H
