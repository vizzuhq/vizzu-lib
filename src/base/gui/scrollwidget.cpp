#include "scrollwidget.h"

#include "base/io/log.h"

#include "movedragobject.h"

using namespace GUI;

ScrollWidget::ScrollWidget(bool horizontal, const Widget *parent)
	: WrapperWidget(parent),
	  horizontal(horizontal)
{
	if (horizontal) setExpand({ true, false });
	else setExpand({ false, true });
}

void ScrollWidget::onUpdateSize(Gfx::ICanvas &canvas, Geom::Size &size)
{
	Geom::Size childSize = size - margin.getSpace();
	auto w = onlyChild.lock();
	if (w)
	{
		w->updateSize(canvas, childSize);

		auto childPos = w->getBoundary().pos;
		auto newChildPos = boundary.pos + margin.topLeft();

		if (horizontal)
		{
			childPos.y = newChildPos.y;

			auto minCoord = newChildPos.x + getContentRect().size.x - childSize.x;
			if (childPos.x < minCoord)
				childPos.x = minCoord;

			if (childPos.x > newChildPos.x) childPos.x = newChildPos.x;
		}
		else
		{
			childPos.x = newChildPos.x;

			auto minCoord = newChildPos.y + getContentRect().size.y - childSize.y;
			if (childPos.y < minCoord)
				childPos.y = minCoord;

			if (childPos.y > newChildPos.y) childPos.y = newChildPos.y;
		}
		w->setPos(childPos);
	}
	auto newSize = childSize + margin.getSpace();

	if (horizontal) newSize.x = size.x;
	else newSize.y = size.y;

	boundary.size = size = newSize;
}

void ScrollWidget::onDraw(Gfx::ICanvas &canvas)
{
	canvas.save();
	canvas.setClipRect(getContentRect());
	WrapperWidget::onDraw(canvas);
	canvas.restore();
}

DragObjectPtr ScrollWidget::onMouseDown(const Geom::Point &pos)
{
	auto res = WrapperWidget::onMouseDown(pos);
	if (!res) return startScroll(pos);
	else return res;
}

bool ScrollWidget::onMouseMove(const Geom::Point &pos, DragObjectPtr &dragObject)
{
	auto deadZone = 15.0;
	auto moveDrag = std::dynamic_pointer_cast<MoveDragObject>(dragObject);
	auto dragOut = std::dynamic_pointer_cast<DragOutObject>(dragObject);
	if ((!moveDrag || moveDrag->getFromWidget().lock() != onlyChild.lock())
		&& !dragOut
		&& dragObject
		&& couldScroll()
		&& ::fabs((dragObject->getStartPos() - pos).getCoord(horizontal)) > deadZone)
	{
		dragObject = startScroll(pos);
		return true;
	}
	return WrapperWidget::onMouseMove(pos, dragObject);
}

bool ScrollWidget::couldScroll() const
{
	auto w = onlyChild.lock();
	if (!w) return false;

	return w->getBoundary().size.getCoord(horizontal)
		> getContentRect().size.getCoord(horizontal);
}

DragObjectPtr ScrollWidget::startScroll(const Geom::Point &pos)
{
	auto res = std::make_shared<MoveDragObject>(pos, onlyChild);
	res->setConstrain({ !horizontal, horizontal });
	return res;
}

void ScrollWidget::scrollTo(std::weak_ptr<Widget> scrollToWidget)
{
	auto w = onlyChild.lock();
	if (!w || !scrollToWidget.lock()) return;

	auto act = getContentRect();
	auto wPos = w->getBoundary().pos;
	auto target = scrollToWidget.lock()->getBoundary();

	if (horizontal)
	{
		if (target.right() > act.right())
			w->setPos(wPos - Geom::Point::X(target.right() - act.right()));

		if (target.left() < act.left())
			w->setPos(wPos - Geom::Point::X(target.left() - act.left()));
	}
	else
	{
		if (target.top() > act.top())
			w->setPos(wPos - Geom::Point::Y(target.top() - act.top()));

		if (target.bottom() < act.bottom())
			w->setPos(wPos - Geom::Point::Y(target.bottom() - act.bottom()));
	}
	scrollToWidget.reset();
}
