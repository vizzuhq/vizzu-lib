#include "drawbackground.h"

#include "chart/main/events.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

DrawBackground::DrawBackground(
    const DrawingContext &context,
    const Geom::Rect &rect,
    const Styles::Box &style,
    const Util::EventDispatcher::event_ptr &onDraw,
    const Util::EventTarget &eventTarget) :
	DrawingContext(context)
{
	Events::OnRectDrawParam eventObj(eventTarget, rect);
	if (!style.borderColor->isTransparent()
	    || !style.backgroundColor->isTransparent()) {
		canvas.setBrushColor(*style.backgroundColor);
		canvas.setLineColor(*style.borderColor);
		canvas.setLineWidth(*style.borderWidth);
		if (!onDraw || onDraw->invoke(std::move(eventObj))) {
			canvas.rectangle(rect);
			renderedChart->emplace(Geom::TransformedRect(rect), eventTarget);
		}
		canvas.setLineWidth(0);
	}
	else if (onDraw)
		onDraw->invoke(std::move(eventObj));
}
