#ifndef GFX_DRAW_ROUNDEDRECT
#define GFX_DRAW_ROUNDEDRECT

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

namespace Gfx::Draw
{

class RoundedRect
{
public:
	RoundedRect(ICanvas &canvas,
	    const Geom::Rect &rect,
	    double radius);

protected:
	void corner(Geom::Point corner,
	    const Geom::Point &dir0,
	    const Geom::Point &dir1);

private:
	ICanvas &canvas;
	double radius;
};

}

#endif
