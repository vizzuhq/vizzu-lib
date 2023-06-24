#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H

#include "drawitem.h"

namespace Vizzu
{
namespace Draw
{

class RectangleItem : public SingleDrawItem
{
public:
	RectangleItem(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    const Styles::Chart &style);
};

static_assert(sizeof(RectangleItem) == sizeof(DrawItem));

}
}

#endif
