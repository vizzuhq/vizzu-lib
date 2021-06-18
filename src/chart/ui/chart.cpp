#include "chart.h"

using namespace Vizzu;
using namespace Vizzu::UI;

ChartWidget::ChartWidget(const GUI::ScreenInfo &screenInfo)
	:
	MainWidget(screenInfo)
{
	chart = std::make_shared<Chart>();
	chart->getEvents().createEvents(chart);
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

bool ChartWidget::onMouseMove(const Geom::Point &pos,
	GUI::DragObjectPtr &dragObject)
{
	mousePos = pos;
	return MainWidget::onMouseMove(pos, dragObject);
}

void ChartWidget::onDraw(Gfx::ICanvas &canvas)
{
	chart->draw(canvas);
}

void ChartWidget::onUpdateSize(Gfx::ICanvas &info, Geom::Size &size)
{
	chart->setBoundRect(Geom::Rect(boundary.pos, size), info);
}
