#include "chart.h"

#include <memory>

#include "base/geom/point.h"
#include "base/gfx/canvas.h"
#include "base/gui/pointer.h"
#include "chart/main/version.h"

#include "events.h"

namespace Vizzu::UI
{

ChartWidget::ChartWidget(
    const std::shared_ptr<Data::DataTable> &table) :
    chart(table),
    onClick(chart.getEventDispatcher().createEvent("click")),
    onPointerMoveEvent(
        chart.getEventDispatcher().createEvent("pointermove")),
    onWheelEvent(chart.getEventDispatcher().createEvent("wheel")),
    onPointerDownEvent(
        chart.getEventDispatcher().createEvent("pointerdown")),
    onPointerUpEvent(
        chart.getEventDispatcher().createEvent("pointerup")),
    onPointerLeaveEvent(
        chart.getEventDispatcher().createEvent("pointerleave"))
{}

void ChartWidget::onPointerDown(const GUI::PointerEvent &event) const
{
	onPointerDownEvent->invoke(PointerEvent{event,
	    chart.getRenderedChart().find(event.position)});
}

void ChartWidget::onPointerMove(const GUI::PointerEvent &event) const
{
	onPointerMoveEvent->invoke(PointerEvent{event,
	    chart.getRenderedChart().find(event.position)});
}

void ChartWidget::onPointerUp(const GUI::PointerEvent &event) const
{
	const auto *eventTarget =
	    chart.getRenderedChart().find(event.position);

	onPointerUpEvent->invoke(PointerEvent{event, eventTarget});

	if (onClick->invoke(PointerEvent{event, eventTarget})) {
		if (chart.getLayout().logo.contains(event.position)) {
			openUrl(Main::siteUrl + std::string("?utm_source=logo"));
		}
	}
}

void ChartWidget::onPointerLeave(const GUI::PointerEvent &event) const
{
	onPointerLeaveEvent->invoke(PointerEvent{event, nullptr});
}

void ChartWidget::onWheel(double delta) const
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