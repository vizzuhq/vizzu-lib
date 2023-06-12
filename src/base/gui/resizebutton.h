#ifndef GUI_RESIZEBUTTON
#define GUI_RESIZEBUTTON

#include <list>

#include "dragobject.h"
#include "widget.h"

namespace GUI
{

class ResizeableWidget;

class ResizerObject;

class ResizeButton : public Widget
{
	friend class ResizerObject;

public:
	ResizeButton(Align verticalPos,
	    Align horizontalPos,
	    ResizeableWidget *controlledWidget,
	    const Widget *parent);

	void resizeParent(const Geom::Point &deltaPos);

	Align getVerticalPos() const { return verticalPos; }
	Align getHorizontalPos() const { return horizontalPos; }

protected:
	bool resizing;
	Align verticalPos;
	Align horizontalPos;
	ResizeableWidget *controlledWidget;

	DragObjectPtr onPointerDown(
	    const GUI::PointerEvent &event) override;
};

class ResizeableWidget : public Widget
{
public:
	ResizeableWidget(const Widget *parent);

	void shiftLeft(double value) { shift.left += value; }
	void shiftTop(double value) { shift.top += value; }
	void shiftRight(double value) { shift.right += value; }
	void shiftBottom(double value) { shift.bottom += value; }
	void resizeTo(Geom::Rect targetRect);

	void onUpdateSize(Gfx::ICanvas &canvas,
	    Geom::Size &size) override;

	void setResizeable(bool enable);

	virtual std::weak_ptr<ResizeButton>
	emplaceResizeButton(Align verticalPos, Align horizontalPos) = 0;

protected:
	Margin shift;
	std::list<std::weak_ptr<ResizeButton>> resizeButtons;

	void populateButtons(bool alsoOnSideCenter = false);
	void updateButtonPositions(Gfx::ICanvas &canvas);
};

}

#endif
