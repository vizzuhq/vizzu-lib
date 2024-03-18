#include "chart.h"

#include "chart/main/version.h"

#include "events.h"

namespace Vizzu::UI
{

ChartWidget::ChartWidget()
{
	chart.onChanged = [this]()
	{
		onChanged();
	};

	auto &ed = chart.getEventDispatcher();
	onClick = ed.createEvent("click");
	onPointerDownEvent = ed.createEvent("pointerdown");
	onPointerUpEvent = ed.createEvent("pointerup");
	onPointerMoveEvent = ed.createEvent("pointermove");
	onPointerLeaveEvent = ed.createEvent("pointerleave");
	onWheelEvent = ed.createEvent("wheel");
}

ChartWidget::~ChartWidget()
{
	auto &ed = chart.getEventDispatcher();
	ed.destroyEvent(onClick);
	ed.destroyEvent(onPointerMoveEvent);
	ed.destroyEvent(onWheelEvent);
	ed.destroyEvent(onPointerDownEvent);
	ed.destroyEvent(onPointerUpEvent);
	ed.destroyEvent(onPointerLeaveEvent);
}

void ChartWidget::onChanged()
{
	if (doChange) doChange();
}

void ChartWidget::onPointerDown(const GUI::PointerEvent &event)
{
	onPointerDownEvent->invoke(PointerEvent{event,
	    chart.getRenderedChart().find(event.position)});
}

void ChartWidget::onPointerMove(const GUI::PointerEvent &event)
{
	onPointerMoveEvent->invoke(PointerEvent{event,
	    chart.getRenderedChart().find(event.position)});
}

void ChartWidget::onPointerUp(const GUI::PointerEvent &event)
{
	const auto *eventTarget =
	    chart.getRenderedChart().find(event.position);

	onPointerUpEvent->invoke(PointerEvent{event, eventTarget});

	if (onClick->invoke(PointerEvent{event, eventTarget})) {
		if (chart.getLayout().logo.contains(event.position)) {
			if (openUrl)
				openUrl(
				    Main::siteUrl + std::string("?utm_source=logo"));
		}
	}
}

void ChartWidget::onPointerLeave(const GUI::PointerEvent &event)
{
	onPointerLeaveEvent->invoke(PointerEvent{event, nullptr});
}

void ChartWidget::onWheel(double delta)
{
	onWheelEvent->invoke(WheelEvent(delta, nullptr));
}

void ChartWidget::onDraw(const std::shared_ptr<Gfx::ICanvas> &canvas)
{
	chart.draw(*canvas);
}

void ChartWidget::onUpdateSize(Geom::Size size)
{
	chart.setBoundRect(Geom::Rect(Geom::Point{}, size));
}

}