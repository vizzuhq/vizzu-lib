#ifndef GUI_POINTER
#define GUI_POINTER

#include <cstdint>
#include <optional>
#include <string>

#include "base/geom/affinetransform.h"
#include "base/geom/point.h"

namespace GUI
{

enum class Cursor : uint16_t { point, push, grab, drag, busy };

static inline const char *toCSS(Cursor cursor)
{
	using C = GUI::Cursor;
	switch (cursor) {
	case C::push:
	case C::grab: return "pointer";
	case C::drag: return "move";
	case C::busy: return "wait";
	case C::point:
	default: return "default";
	};
}

class PointerEvent
{
public:
	PointerEvent() = default;
	PointerEvent(std::optional<int> pointerId,
	    const Geom::Point &pos) :
	    pointerId(pointerId),
	    pos(pos)
	{}
	std::optional<int> pointerId;
	Geom::Point pos;
};

}

#endif
