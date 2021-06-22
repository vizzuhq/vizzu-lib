#include "chart.h"

#include "chart/generator/selector.h"

using namespace Vizzu;
using namespace Vizzu::UI;

ChartWidget::ChartWidget(const GUI::ScreenInfo &screenInfo)
	:
	MainWidget(screenInfo)
{
	chart = std::make_shared<Chart>();
	chart->onChanged = [&]() { onChanged(); };
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
	GUI::DragObjectPtr &dragObject)
{
	mousePos = pos;
	return MainWidget::onMouseMove(pos, dragObject);
}

bool ChartWidget::onMouseUp(const Geom::Point &pos,
    GUI::DragObjectPtr /*dragObject*/)
{
	bool selectionEnabled = true;

	if (selectionEnabled)
	{
		const auto *marker = chart->markerAt(pos);
		auto diagram = chart->getDiagram();
		if (marker)
		{
			Diag::Selector(*diagram).toggleMarker(*marker);
		}
		else
		{
			Diag::Selector(*diagram).clearSelection();
		}
		onChanged();
		return true;
	}
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
