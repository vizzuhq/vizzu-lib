#include "chart.h"

#include <utility>

#include "chart/generator/selector.h"
#include "chart/main/version.h"

#include "events.h"

namespace Vizzu::UI
{

ChartWidget::ChartWidget(GUI::Scheduler &scheduler) :
    scheduler(scheduler)
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
	onPointerOnEvent = ed.createEvent("pointeron");
	onWheelEvent = ed.createEvent("wheel");

	chart.getAnimControl().onComplete.attach(
	    [this]()
	    {
		    if (unprocessedPointerLeave) {
			    onPointerLeave({}, pointerEvent);
			    unprocessedPointerLeave = false;
		    }
		    if (unprocessedPointerMove) {
			    trackMarker();
			    unprocessedPointerMove = false;
		    }
	    });
}

ChartWidget::~ChartWidget()
{
	auto &ed = chart.getEventDispatcher();
	ed.destroyEvent(onClick);
	ed.destroyEvent(onPointerMoveEvent);
	ed.destroyEvent(onPointerOnEvent);
	ed.destroyEvent(onWheelEvent);
	ed.destroyEvent(onPointerDownEvent);
	ed.destroyEvent(onPointerUpEvent);
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
	pointerEvent = event;
	updateCursor(canvas);

	onPointerDownEvent->invoke(PointerEvent(event.pointerId,
	    event.pos,
	    chart.getRenderedChart().find(event.pos)));
}

void ChartWidget::onPointerMove(
    const std::shared_ptr<Gfx::ICanvas> &canvas,
    const GUI::PointerEvent &event)
{
	pointerEvent = event;
	updateCursor(canvas);
	unprocessedPointerLeave = false;
	if (!chart.getAnimControl().isRunning())
		trackMarker();
	else
		unprocessedPointerMove = true, trackedMarkerId.reset();

	onPointerMoveEvent->invoke(PointerEvent(event.pointerId,
	    event.pos,
	    chart.getRenderedChart().find(event.pos)));
}

void ChartWidget::onPointerUp(
    const std::shared_ptr<Gfx::ICanvas> &canvas,
    const GUI::PointerEvent &event)
{
	pointerEvent = event;

	const auto *clickedMarker = getMarkerAt(event.pos);

	onPointerUpEvent->invoke(PointerEvent(event.pointerId,
	    event.pos,
	    chart.getRenderedChart().find(event.pos)));

	if (onClick->invoke(PointerEvent(event.pointerId,
	        event.pos,
	        chart.getRenderedChart().find(event.pos)))) {
		if (chart.getLogoBoundary().contains(pointerEvent.pos)) {
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

	updateCursor(canvas);
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

void ChartWidget::onPointerLeave(
    const std::shared_ptr<Gfx::ICanvas> &,
    const GUI::PointerEvent &)
{
	if (!chart.getAnimControl().isRunning()
	    && reportedMarkerId.has_value()) {
		onPointerOnEvent->invoke(
		    PointerEvent(0, Geom::Point(), nullptr));
		trackedMarkerId.reset(), reportedMarkerId.reset();
	}
	else
		unprocessedPointerLeave = true;
	unprocessedPointerMove = false;
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
    const std::shared_ptr<Gfx::ICanvas> &canvas)
{
	if (chart.getLogoBoundary().contains(pointerEvent.pos))
		return setCursor(canvas, GUI::Cursor::push);

	if (!chart.getAnimControl().isRunning())
		if (auto plot = chart.getPlot())
			if (plot->anySelected || getMarkerAt(pointerEvent.pos))
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

void ChartWidget::trackMarker()
{
	auto plot = chart.getPlot();
	if (!trackedMarkerId.has_value() && plot) {
		if (const auto *clickedMarker =
		        getMarkerAt(pointerEvent.pos)) {
			trackedMarkerId = clickedMarker->idx;
			auto now = std::chrono::steady_clock::now();
			scheduler.get().schedule(
			    [this]()
			    {
				    auto plot = chart.getPlot();
				    const auto *marker =
				        getMarkerAt(pointerEvent.pos);
				    if (marker && trackedMarkerId.has_value()
				        && static_cast<uint64_t>(
				               trackedMarkerId.value())
				               == marker->idx) {
					    Events::Targets::Marker markerElement(
					        *marker);
					    if (reportedMarkerId != trackedMarkerId)
						    onPointerOnEvent->invoke(
						        PointerEvent(pointerEvent.pointerId,
						            pointerEvent.pos,
						            &markerElement));
					    reportedMarkerId = trackedMarkerId;
				    }
				    if (marker == nullptr
				        && reportedMarkerId.has_value()) {
					    onPointerOnEvent->invoke(
					        PointerEvent(pointerEvent.pointerId,
					            pointerEvent.pos,
					            nullptr));
					    reportedMarkerId.reset();
				    }
				    trackedMarkerId.reset();
			    },
			    now);
		}
		else {
			trackedMarkerId.reset();
			if (reportedMarkerId.has_value()) {
				onPointerOnEvent->invoke(
				    PointerEvent(pointerEvent.pointerId,
				        pointerEvent.pos,
				        nullptr));
				reportedMarkerId.reset();
			}
		}
	}
}

}