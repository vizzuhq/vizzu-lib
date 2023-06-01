#ifndef GUI_POINTER
#define GUI_POINTER

#include <cstdint>
#include <string>

#include "base/geom/point.h"
#include "base/geom/affinetransform.h"

namespace GUI
{

enum class Cursor : uint16_t {
	point,
	push,
	grab,
	drag,
	busy
};

static inline const char *toCSS(Cursor cursor)
{
	using C = GUI::Cursor;
	switch (cursor)
	{
	case C::point: return "default";
	case C::push:
	case C::grab: return "pointer";
	case C::drag: return "move";
	case C::busy: return "wait";
	};
	return "default";
}

class PointerEvent {
public:
	PointerEvent() : pointerId(-1) {}
	PointerEvent(int pointerId, const Geom::Point &pos)
	: pointerId(pointerId), pos(pos)
	{}
	PointerEvent transformed(const Geom::AffineTransform &transform) const {
		PointerEvent res(*this);
		res.pos = transform(pos);
		return res;
	}
	int pointerId;
	Geom::Point pos;
};

}

#endif

