#include "drawbackground.h"

#include "chart/main/events.h"

namespace Vizzu::Draw
{

DrawBackground::DrawBackground(const DrawingContext &context,
    const Geom::Rect &rect,
    const Styles::Box &style,
    const Util::EventDispatcher::event_ptr &onDraw,
    std::unique_ptr<Util::EventTarget> eventTarget) :
    DrawingContext(context)
{
	Events::OnRectDrawEvent eventObj(*eventTarget, {rect, false});
	if (!style.borderColor->isTransparent()
	    || !style.backgroundColor->isTransparent()) {
		canvas.setBrushColor(*style.backgroundColor);
		canvas.setLineColor(*style.borderColor);
		canvas.setLineWidth(*style.borderWidth);
		if (!onDraw || onDraw->invoke(std::move(eventObj))) {
			canvas.rectangle(rect);
			renderedChart.emplace(
			    Geom::TransformedRect::fromRect(rect),
			    std::move(eventTarget));
		}
		canvas.setLineWidth(0);
	}
	else if (onDraw)
		onDraw->invoke(std::move(eventObj));
}

}