#include "drawbackground.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

drawBackground::drawBackground(const Geom::Rect &rect,
    Gfx::ICanvas &canvas,
    const Styles::Box &style)
{
	if (!style.borderColor->isTransparent()
		|| !style.backgroundColor->isTransparent())
	{
		canvas.setBrushColor(*style.backgroundColor);
		canvas.setLineColor(*style.borderColor);
		canvas.setLineWidth(*style.borderWidth);
		canvas.rectangle(rect);
		canvas.setLineWidth(0);
	}
}
