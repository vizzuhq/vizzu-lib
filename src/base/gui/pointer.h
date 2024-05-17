#ifndef GUI_POINTER
#define GUI_POINTER

#include <cstdint>
#include <optional>
#include <string>

#include "base/geom/affinetransform.h"
#include "base/geom/point.h"

namespace GUI
{

struct PointerEvent
{
	int pointerId;
	Geom::Point position;
};

}

#endif
