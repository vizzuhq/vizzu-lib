#include "transformerwidget.h"

using namespace GUI;

ShifterObject::ShifterObject(const Geom::Point startPos, const std::weak_ptr<Widget> &fromWidget) :
	DragObject(fromWidget),
	lastPos(startPos)
{
	moveStarted = false;
}

bool ShifterObject::dragMoved(const Geom::Point &pos)
{
	DragObject::dragMoved(pos);

	if (!moveStarted && (pos - lastPos).abs() < 15) {
		return false;
	} else {
		moveStarted = true;
		auto w = std::dynamic_pointer_cast<ShiftableWidget>(getFromWidget().lock());
		if (w) w->shiftCanvas(pos - lastPos);
		lastPos = pos;
		return true;
	}
}

bool ShifterObject::isMoved() const
{
	return moveStarted;
}

TransformerWidget::TransformerWidget(const Widget *parent)
    : ContainerWidget(parent)
{
	transformable = true;
}

Geom::AffineTransform TransformerWidget::getSelfTransform() const
{
	return transform;
}

void TransformerWidget::setSelfTransform(const Geom::AffineTransform &newTransform)
{
	if (transformable) transform = newTransform;
}

void TransformerWidget::resetSelfTransform()
{
	transform = Geom::AffineTransform();
}

DragObjectPtr TransformerWidget::onPointerDown(const GUI::PointerEvent &event)
{
	auto res = ContainerWidget::onPointerDown(event.transformed(transform.inverse()));
	return res;
}

bool TransformerWidget::onPointerUp(const GUI::PointerEvent &event, DragObjectPtr dragObject)
{
	return ContainerWidget::onPointerUp(event.transformed(transform.inverse()), dragObject);
}

bool TransformerWidget::onPointerMove(const GUI::PointerEvent &event, DragObjectPtr &dragObject)
{
	return ContainerWidget::onPointerMove(event.transformed(transform.inverse()), dragObject);
}

std::string TransformerWidget::getHint(const Geom::Point &pos)
{
	return ContainerWidget::getHint(transform.inverse()(pos));
}

void TransformerWidget::onDraw(Gfx::ICanvas &canvas)
{
	if (transform.transforms())
	{
		canvas.save();
		canvas.transform(transform);
		Widget::onDraw(canvas);
		canvas.restore();
	}
	else Widget::onDraw(canvas);
}

void TransformerWidget::setTransformable(bool enable)
{
	transformable = enable;
}

bool TransformerWidget::isTransformable() const
{
	return transformable;
}

ShiftableWidget::ShiftableWidget(const Widget *parent)
    : TransformerWidget(parent)
{
	shiftable = true;
}

DragObjectPtr ShiftableWidget::onPointerDown(const GUI::PointerEvent &event)
{
	auto res = TransformerWidget::onPointerDown(event);
	if (res) return res;

	if (shiftable) return startShift(event.pos);
	else return DragObjectPtr();
}

DragObjectPtr ShiftableWidget::startShift(const Geom::Point &pos)
{
	return std::make_shared<ShifterObject>(pos, shared_from_this());
}

void ShiftableWidget::shiftCanvas(const Geom::Point &delta)
{
	if (shiftable)
	{
		auto shifted = getSelfTransform() + delta;
		setSelfTransform(shifted);
	}
}

void ShiftableWidget::setShiftable(bool enable)
{
	shiftable = enable;
}

bool ShiftableWidget::isShiftable() const
{
	return shiftable;
}

