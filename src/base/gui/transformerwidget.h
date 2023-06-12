#ifndef GUI_TRANSFORMERWIDGET
#define GUI_TRANSFORMERWIDGET

#include "base/geom/affinetransform.h"

#include "dragobject.h"
#include "widget.h"

namespace GUI
{

class TransformerWidget : public ContainerWidget
{
public:
	TransformerWidget(const Widget *parent);

protected:
	Geom::AffineTransform getSelfTransform() const override;
	void setSelfTransform(const Geom::AffineTransform &newTransform);
	void resetSelfTransform();
	void setTransformable(bool enable);
	bool isTransformable() const;
	DragObjectPtr onPointerDown(
	    const GUI::PointerEvent &event) override;
	bool onPointerUp(const GUI::PointerEvent &event,
	    DragObjectPtr dragObject) override;
	bool onPointerMove(const GUI::PointerEvent &event,
	    DragObjectPtr &dragObject) override;
	std::string getHint(const Geom::Point &pos) override;
	void onDraw(Gfx::ICanvas &) override;

private:
	bool transformable;
	Geom::AffineTransform transform;
};

class ShifterObject : public DragObject
{
public:
	ShifterObject(const Geom::Point startPos,
	    const std::weak_ptr<Widget> &fromWidget);

	bool dragMoved(const Geom::Point &pos) override;
	bool isMoved() const;

private:
	bool moveStarted;
	Geom::Point lastPos;
};

class ShiftableWidget : public TransformerWidget
{
public:
	ShiftableWidget(const Widget *parent);
	void shiftCanvas(const Geom::Point &delta);

protected:
	DragObjectPtr onPointerDown(
	    const GUI::PointerEvent &event) override;
	DragObjectPtr startShift(const Geom::Point &pos);
	void setShiftable(bool enable);
	bool isShiftable() const;

private:
	bool shiftable;
};

}

#endif
