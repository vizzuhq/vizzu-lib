#include "drawbackground.h"

#include "chart/main/events.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

DrawBackground::DrawBackground(const Geom::Rect &rect,
    Gfx::ICanvas &canvas,
    const Styles::Box &style,
    const Util::EventDispatcher::event_ptr &onDraw,
    const Util::EventTarget &eventTarget)
{
	Events::OnRectDrawParam eventObj(eventTarget, rect);
	if (!style.borderColor->isTransparent()
	    || !style.backgroundColor->isTransparent()) {
		canvas.setBrushColor(*style.backgroundColor);
		canvas.setLineColor(*style.borderColor);
		canvas.setLineWidth(*style.borderWidth);
		if (!onDraw || onDraw->invoke(std::move(eventObj))) {
			canvas.rectangle(rect);
		}
		canvas.setLineWidth(0);
	}
	else if (onDraw)
		onDraw->invoke(std::move(eventObj));
}
