#include "chart.h"

#include "chart/generator/selector.h"
#include "events.h"

using namespace Vizzu;
using namespace Vizzu::UI;

ChartWidget::ChartWidget()
{
	selectionEnabled = true;

	chart = std::make_shared<Chart>();
	chart->onChanged = [&]() { onChanged(); };

	auto &ed = chart->getEventDispatcher();
	onClick = ed.createEvent("click");
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

	if (allowDefault)
	{
		if (diagram)
		{
			if (clickedMarker)
			{
				Diag::Selector(*diagram).toggleMarker(*clickedMarker);
			}
			else
			{
				Diag::Selector(*diagram).clearSelection();
			}
			onChanged();
		}
	}

	updateMouseCursor();

	return false;
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
	if (chart->getAnimControl().isRunning())
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
