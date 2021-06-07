#include "mainwidget.h"
#include "dragobject.h"

using namespace GUI;

MainWidget::MainWidget(const GUI::ScreenInfo &screenInfo) :
    TransformerWidget(nullptr)
{
	boundary.pos = Geom::Point(0,0);

	if (screenInfo.size_mm.diagonal() < 350)
	{
/*		auto pixPerMM = screenInfo.dpi / 25.4;
		auto desiredPPM = 5;
		transform.scale = pixPerMM / desiredPPM; // also use R3 rounding?
*/	}
}

void MainWidget::onUpdateSize(Gfx::ICanvas &info, Geom::Size &size)
{
	boundary.size = size;
	size = getSelfTransform().inverse()(size);
	for (const auto &child : children) {
		child->updateSize(info, size);
	}
	size = getSelfTransform()(size);
}

DragObjectPtr MainWidget::onMouseDown(const Geom::Point &pos)
{
	dragObject = BaseWidget::onMouseDown(pos);
	if (dragObject) dragObject->dragMoved(pos);
	return dragObject;
}

bool MainWidget::onMouseUp(const Geom::Point &pos, DragObjectPtr)
{
	auto res = BaseWidget::onMouseUp(pos, dragObject);
	dragObject = DragObjectPtr();
	return res;
}

bool MainWidget::onMouseMove(const Geom::Point &pos, DragObjectPtr&)
{
	setCursor(GUI::Cursor::point);

	bool res = false;
	if (dragObject)
	{
		res |= dragObject->dragMoved(pos);
	}
	if (!res) res |= BaseWidget::onMouseMove(pos, dragObject);
	return res;
}

