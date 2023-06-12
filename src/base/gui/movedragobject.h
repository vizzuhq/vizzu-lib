#ifndef GUI_MOVEDRAGOBJECT
#define GUI_MOVEDRAGOBJECT

#include "accessories.h"
#include "dragobject.h"

namespace GUI
{

class MoveDragObject : public DragObject
{
public:
	MoveDragObject(const Geom::Point &startPos,
	    const std::weak_ptr<Widget> &fromWidget);

	bool dragMoved(const Geom::Point &pos) override;

	void setConstrain(const Directions &constrain);
	void setDeadZone(double value);
	bool isMoved() const;

protected:
	Geom::Point lastPos;
	Directions constrain;
	double deadZone;
	bool moveStarted;
};

}

#endif
