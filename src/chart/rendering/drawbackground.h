#ifndef CHART_RENDERING_DRAWBACKGROUND_H
#define CHART_RENDERING_DRAWBACKGROUND_H

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "chart/main/style.h"

namespace Vizzu
{
namespace Draw
{

class drawBackground
{
public:
	drawBackground(const Geom::Rect &rect,
	    Gfx::ICanvas &canvas,
	    const Styles::Box &style);
};

}
}

#endif
