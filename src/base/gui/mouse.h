#ifndef GUI_MOUSE
#define GUI_MOUSE

#include <cstdint>

namespace GUI
{

enum class Cursor : uint16_t {
	point,
	push,
	grab,
	drag,
	busy
};

}

#endif

