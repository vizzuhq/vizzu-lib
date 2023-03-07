#include "drawbackground.h"

#include "chart/main/events.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

drawBackground::drawBackground(const Geom::Rect &rect,
    Gfx::ICanvas &canvas,
    const Styles::Box &style,
	const Util::EventDispatcher::event_ptr &onDraw)
{
	if (!style.borderColor->isTransparent()
		|| !style.backgroundColor->isTransparent())
	{
		canvas.setBrushColor(*style.backgroundColor);
		canvas.setLineColor(*style.borderColor);
		canvas.setLineWidth(*style.borderWidth);
		if (!onDraw || onDraw->invoke(Events::OnRectDrawParam(rect)))
		{
			canvas.rectangle(rect);
		}
		canvas.setLineWidth(0);
	}
	else if(onDraw) onDraw->invoke(Events::OnRectDrawParam(rect));
}
