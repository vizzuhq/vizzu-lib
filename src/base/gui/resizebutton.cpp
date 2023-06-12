#include "resizebutton.h"

namespace GUI
{

class ResizerObject : public DragObject
{
public:
	ResizerObject(const Geom::Point &startPos,
	    const std::weak_ptr<ResizeButton> &resizeButton) :
	    DragObject(resizeButton),
	    lastPos(startPos),
	    resizeButton(resizeButton)
	{}

	~ResizerObject() override
	{
		resizeButton.lock()->resizing = false;
	}

	bool dragMoved(const Geom::Point &pos) override
	{
		DragObject::dragMoved(pos);

		auto widget = getFromWidget().lock();
		if (widget) {
			auto actPos = widget->getTransform().inverse()(pos);
			resizeButton.lock()->resizeParent(actPos - lastPos);
			lastPos = actPos;
			return true;
		}
		else
			return false;
	}

private:
	Geom::Point lastPos;
	std::weak_ptr<ResizeButton> resizeButton;
};

ResizeButton::ResizeButton(Align verticalPos,
    Align horizontalPos,
    ResizeableWidget *controlledWidget,
    const Widget *parent) :
    Widget(parent),
    verticalPos(verticalPos),
    horizontalPos(horizontalPos),
    controlledWidget(controlledWidget)
{
	resizing = false;
}

void ResizeButton::resizeParent(const Geom::Point &deltaPos)
{
	if (verticalPos == Align::Min)
		controlledWidget->shiftBottom(deltaPos.y);

	if (verticalPos == Align::Max)
		controlledWidget->shiftTop(deltaPos.y);

	if (horizontalPos == Align::Min)
		controlledWidget->shiftLeft(deltaPos.x);

	if (horizontalPos == Align::Max)
		controlledWidget->shiftRight(deltaPos.x);
}

DragObjectPtr ResizeButton::onPointerDown(
    const GUI::PointerEvent &event)
{
	resizing = true;
	return std::make_shared<ResizerObject>(event.pos,
	    getAs<ResizeButton>());
}

ResizeableWidget::ResizeableWidget(const Widget *parent) :
    Widget(parent)
{
	shift = {0.0, 0.0, 0.0, 0.0};
}

void ResizeableWidget::resizeTo(Geom::Rect targetRect)
{
	shift.left = targetRect.pos.x - boundary.pos.x;
	shift.bottom = targetRect.pos.y - boundary.pos.y;
	shift.right = targetRect.size.x - boundary.size.x + shift.left;
	shift.top = targetRect.size.y - boundary.size.y + shift.bottom;
}

void ResizeableWidget::populateButtons(bool alsoOnSideCenter)
{
	for (auto v = (int)Align::Min; v <= (int)Align::Max; v++)
		for (auto h = (int)Align::Min; h <= (int)Align::Max; h++) {
			if (v == (int)Align::Center && h == (int)Align::Center)
				continue;

			if (!alsoOnSideCenter
			    && (v == (int)Align::Center
			        || h == (int)Align::Center))
				continue;

			auto button = emplaceResizeButton((Align)v, (Align)h);

			if (button.lock()) resizeButtons.push_back(button);
		}
}

void ResizeableWidget::onUpdateSize(Gfx::ICanvas &canvas,
    Geom::Size &size)
{
	boundary.setLeft(boundary.left() + shift.left);
	boundary.setTop(boundary.top() + shift.top);
	boundary.setRight(boundary.right() + shift.right);
	boundary.setBottom(boundary.bottom() + shift.bottom);

	shift = {0.0, 0.0, 0.0, 0.0};

	size = boundary.size;

	updateButtonPositions(canvas);
}

void ResizeableWidget::setResizeable(bool enable)
{
	for (const auto &item : resizeButtons)
		item.lock()->setEnabled(enable);
}

void ResizeableWidget::updateButtonPositions(Gfx::ICanvas &canvas)
{
	for (const auto &item : resizeButtons) {
		auto button = item.lock();
		Geom::Size buttSize;
		button->updateSize(canvas, buttSize);

		auto horPos = button->getHorizontalPos();
		auto verPos = button->getVerticalPos();

		auto x = horPos == Align::Min ? boundary.left()
		       : horPos == Align::Max
		           ? boundary.right() - buttSize.x
		           : boundary.center().x - buttSize.x / 2;

		auto y = verPos == Align::Min ? boundary.bottom()
		       : verPos == Align::Max
		           ? boundary.top() - buttSize.y
		           : boundary.center().y - buttSize.y / 2;

		button->setPos(Geom::Point(x, y));
	}
}

}
