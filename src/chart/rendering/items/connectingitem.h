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
	ConnectingDrawItem(const Diag::Marker &marker,
	    const Diag::Options &options,
	    const Diag::Plot::Markers &markers,
	    size_t lineIndex,
	    Diag::ShapeType::Type type);

	static const Diag::Marker *getPrev(const Diag::Marker &marker,
	    const Diag::Plot::Markers &markers,
	    size_t lineIndex);

protected:
	size_t lineIndex;
};

}
}

#endif
