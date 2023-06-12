#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include <vector>

#include "base/gfx/pixmap.h"

#include "widget.h"

namespace GUI
{

struct IconDrawing
{
	Gfx::PixMap icon;
	int rotateQuadrant;
};

class IconWidget : public Widget
{
public:
	std::function<bool()> drawDisabled;

	using Widget::Widget;
	void onUpdateSize(Gfx::ICanvas &, Geom::Size &) override;

protected:
	virtual IconDrawing getIcon() = 0;

private:
	void onDraw(Gfx::ICanvas &canvas) override;
};

class Button : public IconWidget
{
public:
	Button(const Widget *parent);

protected:
	DragObjectPtr onPointerDown(
	    const GUI::PointerEvent &event) override;
	bool onPointerMove(const GUI::PointerEvent &event,
	    DragObjectPtr &) override;
	bool onPointerUp(const GUI::PointerEvent &event,
	    DragObjectPtr dragObject) override;
	void dragLeft(const DragObjectPtr &dragObject) override;

	virtual void onPushed() = 0;
	virtual void onReleased() = 0;
	virtual void onTriggered() = 0;
};

class PushButton : public Button
{
public:
	PushButton(const Widget *parent);
	bool isPushed() const { return pushed; }

protected:
	bool pushed;

	IconDrawing getIcon() override;
	void onPushed() override;
	void onReleased() override;

	virtual IconDrawing getIcon(bool pushed) = 0;
};

class ClickButton : public PushButton
{
public:
	using PushButton::PushButton;

private:
	void onTriggered() override;
	virtual void onClicked() = 0;
};

class SelectButton : public ClickButton
{
public:
	SelectButton(const Widget *parent);

protected:
	using ClickButton::getIcon;
	IconDrawing getIcon(bool pushed) override;
	void onClicked() override;

	virtual void onSelectChange(bool selected) = 0;
	virtual bool isSelected() const = 0;
	virtual IconDrawing getIcon(bool selected, bool pushed) = 0;
};

class MultiChoiceButton : public ClickButton
{
public:
	MultiChoiceButton(size_t versions,
	    bool showNext,
	    const Widget *parent);

	size_t nextIndex() const;
	virtual int getIndex() const = 0;

protected:
	size_t versions;
	bool showNext;

private:
	using ClickButton::getIcon;
	IconDrawing getIcon(bool pushed) override;
	void onClicked() override;

	virtual IconDrawing getIcon(int index, bool pushed) = 0;
	virtual void onIndexChange(int index) = 0;
};

class RadioButton : public ClickButton
{
public:
	struct Group
	{
		std::weak_ptr<Widget> selected;
	};
	typedef std::shared_ptr<Group> GroupPtr;
	static GroupPtr makeGroup();

	RadioButton(GroupPtr group, const Widget *parent);
	void select();
	bool isSelected() const;

protected:
	GroupPtr group;

private:
	using ClickButton::getIcon;
	IconDrawing getIcon(bool pushed) override;
	void onClicked() override;

	virtual void onSelected() = 0;
	virtual IconDrawing getIcon(bool selected, bool pushed) = 0;
};

}

#endif
