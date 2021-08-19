#include "mainwidget.h"
#include "dragobject.h"

using namespace GUI;

MainWidget::MainWidget() : ContainerWidget(nullptr)
{
	boundary.pos = Geom::Point(0,0);
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

