#include "chart.h"

#include <utility>

#include "chart/generator/selector.h"
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
	needUpdate = true;
}

void ChartWidget::setCursor(
    const std::shared_ptr<Gfx::ICanvas> &canvas,
    GUI::Cursor cursor) const
{
	if (doSetCursor) doSetCursor(canvas, cursor);
}

void ChartWidget::onPointerDown(
    const std::shared_ptr<Gfx::ICanvas> &canvas,
    const GUI::PointerEvent &event)
{
	updateCursor(canvas, event.pos);

	onPointerDownEvent->invoke(PointerEvent(event.pointerId,
	    event.pos,
	    chart.getRenderedChart().find(event.pos)));
}

void ChartWidget::onPointerMove(
    const std::shared_ptr<Gfx::ICanvas> &canvas,
    const GUI::PointerEvent &event)
{
	updateCursor(canvas, event.pos);

	onPointerMoveEvent->invoke(PointerEvent(event.pointerId,
	    event.pos,
	    chart.getRenderedChart().find(event.pos)));
}

void ChartWidget::onPointerUp(
    const std::shared_ptr<Gfx::ICanvas> &canvas,
    const GUI::PointerEvent &event)
{
	const auto *clickedMarker = getMarkerAt(event.pos);

	onPointerUpEvent->invoke(PointerEvent(event.pointerId,
	    event.pos,
	    chart.getRenderedChart().find(event.pos)));

	if (onClick->invoke(PointerEvent(event.pointerId,
	        event.pos,
	        chart.getRenderedChart().find(event.pos)))) {
		if (chart.getLogoBoundary().contains(event.pos)) {
			if (openUrl)
				openUrl(
				    Main::siteUrl + std::string("?utm_source=logo"));
		}
		else if (auto plot = chart.getPlot()) {
			if (clickedMarker)
				Gen::Selector(*plot).toggleMarker(
				    const_cast<Gen::Marker &>( // NOLINT
				        *clickedMarker));
			else
				Gen::Selector(*plot).clearSelection();
			onChanged();
		}
	}

	updateCursor(canvas, event.pos);
}

void ChartWidget::onPointerLeave(
    const std::shared_ptr<Gfx::ICanvas> &canvas,
    const GUI::PointerEvent &event)
{
	updateCursor(canvas, event.pos);

	onPointerLeaveEvent->invoke(
	    PointerEvent(event.pointerId, event.pos, nullptr));
}

void ChartWidget::onWheel(const std::shared_ptr<Gfx::ICanvas> &,
    double delta)
{
	onWheelEvent->invoke(WheelEvent(delta, nullptr));
}

Geom::Size ChartWidget::getSize(
    const std::shared_ptr<Gfx::ICanvas> &) const
{
	return chart.getLayout().boundary.size;
}

void ChartWidget::onDraw(const std::shared_ptr<Gfx::ICanvas> &canvas)
{
	chart.draw(*canvas);
	needUpdate = false;
}

void ChartWidget::onUpdateSize(
    const std::shared_ptr<Gfx::ICanvas> &canvas,
    Geom::Size size)
{
	chart.setBoundRect(Geom::Rect(Geom::Point{}, size), *canvas);
}

void ChartWidget::updateCursor(
    const std::shared_ptr<Gfx::ICanvas> &canvas,
    const Geom::Point &pos)
{
	if (chart.getLogoBoundary().contains(pos))
		return setCursor(canvas, GUI::Cursor::push);

	if (!chart.getAnimControl().isRunning())
		if (auto plot = chart.getPlot())
			if (plot->anySelected || getMarkerAt(pos))
				return setCursor(canvas, GUI::Cursor::push);
	return setCursor(canvas, GUI::Cursor::point);
}

const Gen::Marker *ChartWidget::getMarkerAt(const Geom::Point &pos)
{
	const auto *element =
	    static_cast<const Events::Targets::Element *>(
	        chart.getRenderedChart().find(pos));

	return element->tagName == "plot-marker"
	         ? &static_cast<const Events::Targets::Marker *>(element)
	                ->marker
	         : nullptr;
}

}