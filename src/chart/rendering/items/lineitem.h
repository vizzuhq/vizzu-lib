#ifndef LINEITEM_H
#define LINEITEM_H

#include "chart/rendering/painter/coordinatesystem.h"

#include "connectingitem.h"

namespace Vizzu
{
namespace Draw
{

class LineItem : public ConnectingDrawItem
{
public:
	LineItem(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    const Styles::Chart &style,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex);
};

static_assert(sizeof(LineItem) == sizeof(DrawItem));

}
}

#endif
