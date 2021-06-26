#ifndef GUI_MOUSE
#define GUI_MOUSE

#include <cstdint>
#include <string>

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

}

#endif

