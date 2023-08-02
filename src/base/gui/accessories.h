#ifndef GUI_ACCESSORIES
#define GUI_ACCESSORIES

#include "base/geom/point.h"

namespace GUI
{
struct Margin
{
	double top;
	double left;
	double bottom;
	double right;
	[[nodiscard]] Geom::Size getSpace() const
	{
		return {left + right, top + bottom};
	}
	[[nodiscard]] Geom::Point topLeft() const { return {left, top}; }
	[[nodiscard]] Geom::Point bottomRight() const
	{
		return {right, bottom};
	}
};
}

#endif
