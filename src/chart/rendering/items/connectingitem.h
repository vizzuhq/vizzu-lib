#ifndef CONNECTINGITEM_H
#define CONNECTINGITEM_H

#include "drawitem.h"

namespace Vizzu
{
namespace Draw
{

class ConnectingItem : public DrawItem
{
public:
	ConnectingItem(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    const Styles::Chart &style,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex,
	    Gen::ShapeType type);

	static const Gen::Marker *getPrev(const Gen::Marker &marker,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex);
};

}
}

#endif
