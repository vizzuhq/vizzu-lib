#include "chart.h"

#include <utility>

#include "chart/generator/selector.h"
#include "chart/main/version.h"

#include "events.h"

using namespace Vizzu;
using namespace Vizzu::UI;

ChartWidget::ChartWidget(GUI::SchedulerPtr scheduler) :
    chart(),
    scheduler(std::move(scheduler)),
    unprocessedPointerMove(false),
    unprocessedPointerLeave(false),
    trackedMarkerId(-1),
    reportedMarkerId(-1)
{
	selectionEnabled = true;

	chart.onChanged = [&]()
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
	    [&]()
	    {
		    if (unprocessedPointerLeave) {
			    onPointerLeave(pointerEvent);
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

void ChartWidget::onChanged() const
{
	if (doChange) doChange();
}

void ChartWidget::setCursor(GUI::Cursor cursor) const
{
	if (doSetCursor) doSetCursor(cursor);
}

void ChartWidget::onPointerDown(
    const GUI::PointerEvent &event)
{
	pointerEvent = event;
	updateCursor();

	onPointerDownEvent->invoke(PointerEvent(event.pointerId,
	    event.pos,
	    chart.markerAt(event.pos),
	    chart));
}

void ChartWidget::onPointerMove(const GUI::PointerEvent &event)
{
	pointerEvent = event;
	updateCursor();
	unprocessedPointerLeave = false;
	if (!chart.getAnimControl().isRunning())
		trackMarker();
	else
		unprocessedPointerMove = true, trackedMarkerId = -1;

	onPointerMoveEvent->invoke(
	    PointerEvent(event.pointerId, event.pos, nullptr, chart));
}

void ChartWidget::onPointerUp(const GUI::PointerEvent &event)
{
	pointerEvent = event;

	auto plot = chart.getPlot();

	auto *clickedMarker = chart.markerAt(event.pos);

	onPointerUpEvent->invoke(PointerEvent(event.pointerId,
	    event.pos,
	    clickedMarker,
	    chart));

	auto allowDefault = onClick->invoke(PointerEvent(event.pointerId,
	    event.pos,
	    clickedMarker,
	    chart));

	if (allowDefault) {
		if (chart.getLogoBoundary().contains(pointerEvent.pos)) {
			if (openUrl)
				openUrl(
				    Main::siteUrl + std::string("?utm_source=logo"));
		}
		else if (plot) {
			if (clickedMarker)
				Gen::Selector(*plot).toggleMarker(*clickedMarker);
			else
				Gen::Selector(*plot).clearSelection();
			onChanged();
		}
	}

	updateCursor();
}

void ChartWidget::onWheel(double delta)
{
	onWheelEvent->invoke(WheelEvent(delta, chart));
}

Geom::Size ChartWidget::getSize() const
{
	return chart.getLayout().boundary.size;
}

void ChartWidget::onPointerLeave(const GUI::PointerEvent &)
{
	if (!chart.getAnimControl().isRunning()
	    && reportedMarkerId != -1) {
		onPointerOnEvent->invoke(
		    PointerEvent(0, Geom::Point(), nullptr, chart));
		trackedMarkerId = -1, reportedMarkerId = -1;
	}
	else
		unprocessedPointerLeave = true;
	unprocessedPointerMove = false;
}

void ChartWidget::onDraw(Gfx::ICanvas &canvas)
{
	chart.draw(canvas);
}

void ChartWidget::onUpdateSize(Gfx::ICanvas &info, Geom::Size size)
{
	chart.setBoundRect(Geom::Rect(Geom::Point{}, size), info);
}

void ChartWidget::updateCursor()
{
	if (chart.getLogoBoundary().contains(pointerEvent.pos)) {
		setCursor(GUI::Cursor::push);
	}
	else if (chart.getAnimControl().isRunning()) {
		setCursor(GUI::Cursor::point);
	}
	else if (selectionEnabled) {
		auto plot = chart.getPlot();
		if (!plot) { setCursor(GUI::Cursor::point); }
		else {
			const auto *marker = chart.markerAt(pointerEvent.pos);

			if (marker)
				setCursor(GUI::Cursor::push);
			else if (plot->anySelected)
				setCursor(GUI::Cursor::push);
			else
				setCursor(GUI::Cursor::point);
		}
	}
	else {
		setCursor(GUI::Cursor::point);
	}
}

void ChartWidget::trackMarker()
{
	auto plot = chart.getPlot();
	if (trackedMarkerId == -1 && plot) {
		auto clickedMarker = chart.markerAt(pointerEvent.pos);
		if (clickedMarker) {
			trackedMarkerId = clickedMarker->idx;
			auto now = std::chrono::steady_clock::now();
			scheduler->schedule(
			    [&]()
			    {
				    auto plot = chart.getPlot();
				    auto marker = chart.markerAt(pointerEvent.pos);
				    if (marker
				        && static_cast<uint64_t>(trackedMarkerId) == marker->idx) {
					    if (reportedMarkerId != trackedMarkerId)
						    onPointerOnEvent->invoke(
						        PointerEvent(pointerEvent.pointerId,
						            pointerEvent.pos,
						            marker,
						            chart));
					    reportedMarkerId = trackedMarkerId;
				    }
				    if (marker == nullptr && reportedMarkerId != -1) {
					    onPointerOnEvent->invoke(
					        PointerEvent(pointerEvent.pointerId,
					            pointerEvent.pos,
					            nullptr,
					            chart));
					    reportedMarkerId = -1;
				    }
				    trackedMarkerId = -1;
			    },
			    now);
		}
		else {
			trackedMarkerId = -1;
			if (reportedMarkerId != -1) {
				onPointerOnEvent->invoke(
				    PointerEvent(pointerEvent.pointerId,
				        pointerEvent.pos,
				        nullptr,
				        chart));
				reportedMarkerId = -1;
			}
		}
	}
}
