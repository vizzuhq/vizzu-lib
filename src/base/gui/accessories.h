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
	Geom::Size getSpace() const
	{
		return {left + right, top + bottom};
	}
	Geom::Point topLeft() const { return {left, top}; }
	Geom::Point bottomRight() const { return {right, bottom}; }
};
}

#endif
