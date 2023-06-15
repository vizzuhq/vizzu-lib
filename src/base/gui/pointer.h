#ifndef GUI_POINTER
#define GUI_POINTER

#include <cstdint>
#include <string>

#include "base/geom/affinetransform.h"
#include "base/geom/point.h"
#include "base/refl/enum.h"

namespace GUI
{

class SpecNameEnum(Cursor)(point, push, grab, drag, busy)
    ("default,pointer,pointer,move,wait");

class PointerEvent
{
public:
	PointerEvent() : pointerId(-1) {}
	PointerEvent(int pointerId, const Geom::Point &pos) :
	    pointerId(pointerId),
	    pos(pos)
	{}
	int pointerId;
	Geom::Point pos;
};

}

#endif
