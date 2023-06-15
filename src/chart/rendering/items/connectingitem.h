#ifndef CONNECTINGITEM_H
#define CONNECTINGITEM_H

#include "drawitem.h"

namespace Vizzu
{
namespace Draw
{

class ConnectingDrawItem : public DrawItem
{
public:
	ConnectingDrawItem(const Gen::Marker &marker,
	    const Gen::Options &options,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex,
	    Gen::ShapeType::Type type);

	static const Gen::Marker *getPrev(const Gen::Marker &marker,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex);

protected:
	size_t lineIndex;
};

}
}

#endif
