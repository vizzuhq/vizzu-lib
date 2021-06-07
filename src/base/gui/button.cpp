#include "button.h"

#include "base/io/log.h"
#include "dragobject.h"

using namespace GUI;

void IconWidget::onDraw(Gfx::ICanvas &)
{
	try {
		// todo: separated image loading and drawing through ICanvas interface
		/*auto opacity = drawDisabled && drawDisabled() ? 0.3 : 1.0;
		auto icon = getIcon();
		canvas.drawPixmap(getContentRect(),
		                  icon.icon,
		                  icon.rotateQuadrant,
		                  opacity);*/
	}
	catch(std::logic_error &e)
	{
		IO::log() << "Icon widget error: " << e.what();
	}
}

void IconWidget::onUpdateSize(Gfx::ICanvas &, Geom::Size &size)
{
	try {
		size = getIcon().icon.size + margin.getSpace();
		boundary.size = size;
	}
	catch(std::logic_error &e) {
		IO::log() << "Icon widget error: " << e.what();
	}
}

Button::Button(const Widget *parent)
	: IconWidget(parent)
{}

DragObjectPtr Button::onMouseDown(const Geom::Point &)
{
	if (isInteractive()) onPushed();
	return std::make_shared<DragObject>(shared_from_this());
}

bool Button::onMouseMove(const Geom::Point &, DragObjectPtr &)
{
	if (isInteractive()) setCursor(Cursor::push);
	return false;
}

bool Button::onMouseUp(const Geom::Point &, DragObjectPtr dragObject)
{
	if (isInteractive()
		&& dragObject
		&& dragObject->getFromWidget().lock() == shared_from_this())
	{
		onReleased();
		onTriggered();
		return true;
	}
	return false;
}

void Button::dragLeft(const DragObjectPtr &dragObject)
{
	onReleased();
	Widget::dragLeft(dragObject);
}

PushButton::PushButton(const Widget *parent)
	: Button(parent)
{
	pushed = false;
}

IconDrawing PushButton::getIcon()
{
	return getIcon(pushed);
}

void PushButton::onPushed()
{
	pushed = true;
	onChanged();
}

void PushButton::onReleased()
{
	pushed = false;
	onChanged();
}

void ClickButton::onTriggered()
{
	onClicked();
	onChanged();
}

SelectButton::SelectButton(const Widget *parent)
	: ClickButton(parent)
{}

IconDrawing SelectButton::getIcon(bool pushed)
{
	return getIcon(isSelected(), pushed);
}

void SelectButton::onClicked()
{
	onSelectChange(!isSelected());
}

MultiChoiceButton::MultiChoiceButton(size_t versions, bool showNext, const Widget *parent)
	: ClickButton(parent), versions(versions), showNext(showNext)
{
}

IconDrawing MultiChoiceButton::getIcon(bool pushed)
{
	return getIcon((getIndex() + (showNext ? 1 : 0)) % versions, pushed);
}

void MultiChoiceButton::onClicked()
{
	onIndexChange((int)nextIndex());
}

size_t MultiChoiceButton::nextIndex() const
{
	return (getIndex() + 1) % versions;
}

RadioButton::GroupPtr RadioButton::makeGroup()
{
	return std::make_shared<Group>();
}

RadioButton::RadioButton(RadioButton::GroupPtr group,
						 const Widget *parent)
	: ClickButton(parent),
	  group(std::move(group))
{}

void RadioButton::select()
{
	group->selected = shared_from_this();
	onSelected();
}

bool RadioButton::isSelected() const
{
	return group && group->selected.lock() == shared_from_this();
}

IconDrawing RadioButton::getIcon(bool pushed)
{
	return getIcon(isSelected(), pushed);
}

void RadioButton::onClicked()
{
	select();
}
