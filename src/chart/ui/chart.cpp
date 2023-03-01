#include "chart.h"

#include "chart/generator/selector.h"
#include "events.h"
#include "chart/main/version.h"

using namespace Vizzu;
using namespace Vizzu::UI;

ChartWidget::ChartWidget(GUI::SchedulerPtr scheduler)
	:
	scheduler(scheduler),
	unprocessedMouseMove(false), unprocessedMouseLeave(false),
	trackedMarkerId(-1), reportedMarkerId(-1)
{
	selectionEnabled = true;

	chart = std::make_shared<Chart>();
	chart->onChanged = [&]() { onChanged(); };

	auto &ed = chart->getEventDispatcher();
	onClick = ed.createEvent("click");
	onMouseDownEvent = ed.createEvent("mousedown");
	onMouseUpEvent = ed.createEvent("mouseup");
	onMouseMoveEvent = ed.createEvent("mousemove");
	onMouseOnEvent = ed.createEvent("mouseon");
	onMouseWheelEvent = ed.createEvent("wheel");

	chart->getAnimControl().onComplete.attach([&]() {
		if (unprocessedMouseLeave) {
			onMouseLeave();
			unprocessedMouseLeave = false;
		}
		if (unprocessedMouseMove) {
			trackMarker();
			unprocessedMouseMove = false;	
		}
	});
}

ChartWidget::~ChartWidget()
{
	auto &ed = chart->getEventDispatcher();
	ed.destroyEvent(onClick);
	ed.destroyEvent(onMouseMoveEvent);
	ed.destroyEvent(onMouseOnEvent);
	ed.destroyEvent(onMouseWheelEvent);
	ed.destroyEvent(onMouseDownEvent);
	ed.destroyEvent(onMouseUpEvent);
}

void ChartWidget::onChanged() const
{
	if (doChange) doChange();
}

void ChartWidget::setCursor(GUI::Cursor cursor) const
{
	if (setMouseCursor) setMouseCursor(cursor);
}

GUI::DragObjectPtr ChartWidget::onMouseDown(const Geom::Point &pos)
{
	mousePos = pos;
	updateMouseCursor();

	onMouseDownEvent->invoke(MouseEvent(pos, chart->markerAt(pos), *chart));

	return GUI::DragObjectPtr();
}

bool ChartWidget::onMouseMove(const Geom::Point &pos,
	GUI::DragObjectPtr &/*dragObject*/)
{
	mousePos = pos;
	updateMouseCursor();
	unprocessedMouseLeave = false;
	if (!chart->getAnimControl().isRunning())
		trackMarker();
	else
		unprocessedMouseMove = true, trackedMarkerId = -1;

	onMouseMoveEvent->invoke(MouseEvent(mousePos, nullptr, *chart));
	return false;
}

bool ChartWidget::onMouseUp(const Geom::Point &pos,
    GUI::DragObjectPtr /*dragObject*/)
{
	mousePos = pos;

	auto diagram = chart->getDiagram();

	auto *clickedMarker = chart->markerAt(pos);

	onMouseUpEvent->invoke(MouseEvent(pos, clickedMarker, *chart));

	auto allowDefault =
	    onClick->invoke(MouseEvent(pos, clickedMarker, *chart));

	if (allowDefault) {
		if (chart->getLogoBoundary().contains(mousePos)) {
			if (openUrl) openUrl(Main::siteUrl + std::string("?utm_source=logo"));
		}
		else if (diagram) {
			if (clickedMarker)
				Diag::Selector(*diagram).toggleMarker(*clickedMarker);
			else
				Diag::Selector(*diagram).clearSelection();
			onChanged();
		}
	}

	updateMouseCursor();

	return false;
}

bool ChartWidget::onMouseWheel(double delta) 
{
	onMouseWheelEvent->invoke(WheelEvent(delta, *chart));

	return false;
}

void ChartWidget::onMouseLeave() {
	if (!chart->getAnimControl().isRunning() && reportedMarkerId != -1) {
		onMouseOnEvent->invoke(MouseEvent(Geom::Point(), nullptr, *chart));
		trackedMarkerId = -1, reportedMarkerId = -1;
	}
	else
		unprocessedMouseLeave = true;
	unprocessedMouseMove = false;
}

void ChartWidget::onDraw(Gfx::ICanvas &canvas)
{
	chart->draw(canvas);
}

void ChartWidget::onUpdateSize(Gfx::ICanvas &info, Geom::Size &size)
{
	chart->setBoundRect(Geom::Rect(boundary.pos, size), info);
}

void ChartWidget::updateMouseCursor()
{
	if (setMouseCursor) 
	{
		if (chart->getLogoBoundary().contains(mousePos))
		{
			setMouseCursor(GUI::Cursor::push);
		}
		else if (chart->getAnimControl().isRunning())
		{
			setMouseCursor(GUI::Cursor::point);
		}
		else if (selectionEnabled)
		{
			auto diagram = chart->getDiagram();
			if (!diagram)
			{
				setMouseCursor(GUI::Cursor::point);
			}
			else
			{
				const auto *marker = chart->markerAt(mousePos);

				if (marker)
					setMouseCursor(GUI::Cursor::push);
				else if (diagram->anySelected)
					setMouseCursor(GUI::Cursor::push);
				else
					setMouseCursor(GUI::Cursor::point);
			}
		}
		else
		{
			setMouseCursor(GUI::Cursor::point);
		}
	}
}

void ChartWidget::trackMarker() {
	auto diagram = chart->getDiagram();
	if (trackedMarkerId == -1 && diagram) {
		auto clickedMarker = chart->markerAt(mousePos);
		if (clickedMarker) {
			trackedMarkerId = clickedMarker->idx;
			auto now = std::chrono::steady_clock::now();
			scheduler->schedule([&]() {
				auto diagram = chart->getDiagram();
				auto marker = chart->markerAt(mousePos);
				if (marker && (uint64_t)trackedMarkerId == marker->idx) {
					if (reportedMarkerId != trackedMarkerId)
						onMouseOnEvent->invoke(MouseEvent(mousePos, marker, *chart));
					reportedMarkerId = trackedMarkerId;
				}
				if (marker == nullptr && reportedMarkerId != -1) {
					onMouseOnEvent->invoke(MouseEvent(mousePos, nullptr, *chart));
					reportedMarkerId = -1;
				}
				trackedMarkerId = -1;
			},
			now);
		}
		else {
			trackedMarkerId = -1;
			if (reportedMarkerId != -1) {
				onMouseOnEvent->invoke(MouseEvent(mousePos, nullptr, *chart));
				reportedMarkerId = -1;
			}
		}
	}
}
