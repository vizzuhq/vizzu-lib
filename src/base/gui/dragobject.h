#ifndef GUI_DRAGOBJECT
#define GUI_DRAGOBJECT

#include <memory>

#include "base/geom/affinetransform.h"
#include "base/geom/point.h"
#include "widget.h"

namespace GUI
{

class DragObject : public Widget
{
public:
	DragObject(const std::weak_ptr<Widget> &fromWidget);

	virtual ~DragObject() {}

	virtual std::weak_ptr<Widget> getFromWidget() const;
	virtual std::weak_ptr<Widget> getOveredWidget() const;
	virtual void updateOvered(std::weak_ptr<Widget> widget);
	virtual bool dragMoved(const Geom::Point &pos);
	virtual std::shared_ptr<Widget> getWidget() const;

	using Widget::getHint;
	virtual std::string getHint() const;
	Geom::Point getStartPos() const;
	bool isLeftOrigin() const;

protected:
	bool leftFromWidget;
	Geom::Point startPos;
	std::weak_ptr<Widget> fromWidget;
	std::weak_ptr<Widget> overedWidget;
};

class DragOutObject : public DragObject
{
public:
	using DragObject::DragObject;
};

}

#endif
