#ifndef AREAITEM_H
#define AREAITEM_H

#include "connectingitem.h"

namespace Vizzu
{
namespace Draw
{

class AreaItem : public ConnectingDrawItem
{
public:
	AreaItem(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex);
};

static_assert(sizeof(AreaItem) == sizeof(DrawItem));

}
}

#endif
