#ifndef GUI_SCREENINFO
#define GUI_SCREENINFO

#include "base/geom/point.h"

namespace GUI
{

struct ScreenInfo
{
	double dpi;
	Geom::Size size_mm;
};

}

#endif
