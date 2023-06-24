#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include "chart/rendering/painter/coordinatesystem.h"

#include "drawitem.h"

namespace Vizzu
{
namespace Draw
{

class CircleItem : public SingleDrawItem
{
public:
	CircleItem(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    const Styles::Chart &style);
};

static_assert(sizeof(CircleItem) == sizeof(DrawItem));

}
}

#endif
