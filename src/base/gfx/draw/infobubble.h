#ifndef GFX_DRAW_INFOBUBBLE
#define GFX_DRAW_INFOBUBBLE

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

namespace Gfx
{
namespace Draw
{

class InfoBubble
{	
public:
	InfoBubble(ICanvas &canvas, const Geom::Rect &rect, double radius,
		double arrowWidth, const Geom::Point& arrowhead);

private:
	ICanvas &canvas;
	double radius;

	void corner(const Geom::Point corner,
		const Geom::Point &dir0,
		const Geom::Point &dir1);
};

}
}

#endif //GFX_DRAW_INFOBUBBLE
