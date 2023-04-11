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

DragObjectPtr MainWidget::onPointerDown(const GUI::PointerEvent &event)
{
	dragObject = BaseWidget::onPointerDown(event);
	if (dragObject) dragObject->dragMoved(event.pos);
	return dragObject;
}

bool MainWidget::onPointerUp(const GUI::PointerEvent &event, DragObjectPtr)
{
	auto res = BaseWidget::onPointerUp(event, dragObject);
	dragObject = DragObjectPtr();
	return res;
}

bool MainWidget::onPointerMove(const GUI::PointerEvent &event, DragObjectPtr&)
{
	setCursor(GUI::Cursor::point);

	bool res = false;
	if (dragObject)
	{
		res |= dragObject->dragMoved(event.pos);
	}
	if (!res) res |= BaseWidget::onPointerMove(event, dragObject);
	return res;
}

