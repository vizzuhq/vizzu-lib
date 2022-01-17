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
	onMouseMoveEvent = ed.createEvent("mousemove");
	onMouseOnEvent = ed.createEvent("mouseon");

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
}

void ChartWidget::onChanged() const
{
	if (doChange) doChange();
}

void ChartWidget::setCursor(GUI::Cursor cursor) const
{
	if (setMouseCursor) setMouseCursor(cursor);
}

GUI::DragObjectPtr ChartWidget::onMouseDown(const Geom::Point &)
{
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

	onMouseMoveEvent->invoke();
	return false;
}

bool ChartWidget::onMouseUp(const Geom::Point &pos,
    GUI::DragObjectPtr /*dragObject*/)
{
	mousePos = pos;

	const Diag::Marker *clickedMarker = nullptr;

	auto diagram = chart->getDiagram();

	if (diagram) clickedMarker = chart->markerAt(pos);

	auto allowDefault =
	    onClick->invoke(ClickEvent(pos, clickedMarker, *chart));

	if (allowDefault) {
		if (chart->getLogoBoundary().contains(mousePos)) {
			if (openUrl) openUrl(Main::siteUrl);
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

void ChartWidget::onMouseLeave() {
	if (!chart->getAnimControl().isRunning() && reportedMarkerId != -1) {
		onMouseOnEvent->invoke(MouseOnEvent(*chart, nullptr));
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
						onMouseOnEvent->invoke(MouseOnEvent(*chart, marker));
					reportedMarkerId = trackedMarkerId;
				}
				if (marker == nullptr && reportedMarkerId != -1) {
					onMouseOnEvent->invoke(MouseOnEvent(*chart, nullptr));
					reportedMarkerId = -1;
				}
				trackedMarkerId = -1;
			},
			now);
		}
		else {
			trackedMarkerId = -1;
			if (reportedMarkerId != -1) {
				onMouseOnEvent->invoke(MouseOnEvent(*chart, nullptr));
				reportedMarkerId = -1;
			}
		}
	}
}
