#ifndef GFX_DRAW_INFOBUBBLE
#define GFX_DRAW_INFOBUBBLE

#include "base/geom/rect.h"
#include "roundedrect.h"
#include "base/gfx/canvas.h"

namespace Gfx
{
namespace Draw
{

class InfoBubble : RoundedRect
{	
public:
	InfoBubble(ICanvas &canvas, const Geom::Rect &rect, double radius,
		double arrowWidth, const Geom::Point& arrowhead);
};

}
}

#endif //GFX_DRAW_INFOBUBBLE
