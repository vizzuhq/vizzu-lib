#include "dragobject.h"

#include "widget.h"

using namespace GUI;

DragObject::DragObject(const std::weak_ptr<Widget> &fromWidget) :
    Widget(fromWidget.lock().get()),
    fromWidget(fromWidget),
    overedWidget(fromWidget)
{
	leftFromWidget = false;
	startPos = Geom::Point::Invalid();
}

std::weak_ptr<Widget> DragObject::getFromWidget() const
{
	return fromWidget;
}

std::weak_ptr<Widget> DragObject::getOveredWidget() const
{
	return overedWidget;
}

std::shared_ptr<Widget> DragObject::getWidget() const
{
	return std::shared_ptr<Widget>();
}

std::string DragObject::getHint() const { return ""; }

Geom::Point DragObject::getStartPos() const { return startPos; }

bool DragObject::isLeftOrigin() const { return leftFromWidget; }

void DragObject::updateOvered(std::weak_ptr<Widget> widget)
{
	overedWidget = std::move(widget);
}

bool DragObject::dragMoved(const Geom::Point &pos)
{
	if (!startPos.isValid()) startPos = pos;

	setPos(pos - getBoundary().size.xComp() / 2);

	if (!fromWidget.lock()
	    || !fromWidget.lock()->getBoundary().contains(
	        fromWidget.lock()->getTransform().inverse()(pos)))
		leftFromWidget = true;

	auto overed = getOveredWidget().lock();
	if (overed
	    && !overed->getBoundary().contains(
	        overed->getTransform().inverse()(pos))) {
		overed->dragLeft(getAs<DragObject>());
		updateOvered(std::weak_ptr<Widget>());
	}
	return false;
}
