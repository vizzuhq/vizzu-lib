#include "movedragobject.h"

#include "widget.h"

using namespace GUI;

MoveDragObject::MoveDragObject(const Geom::Point &startPos,
    const std::weak_ptr<Widget> &fromWidget) :
    DragObject(fromWidget),
    lastPos(startPos),
    constrain{false, false},
    deadZone(0.0),
    moveStarted(false)
{}

bool MoveDragObject::dragMoved(const Geom::Point &pos)
{
	auto res = DragObject::dragMoved(pos);

	auto widget = getFromWidget().lock();
	if (widget) {
		auto newPos = widget->getTransform().inverse()(pos);

		if (!moveStarted && (newPos - lastPos).abs() < deadZone) {
			return false;
		}
		else {
			moveStarted = true;
			auto diff = newPos - lastPos;
			if (constrain.horizontal) diff.x = 0;
			if (constrain.vertical) diff.y = 0;
			widget->setPos(widget->getPos() + diff);
			lastPos = newPos;
			return true;
		}
	}
	else
		return res;
}

bool MoveDragObject::isMoved() const { return moveStarted; }

void MoveDragObject::setConstrain(const Directions &constrain)
{
	this->constrain = constrain;
}

void MoveDragObject::setDeadZone(double value) { deadZone = value; }
