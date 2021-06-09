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
	    const Diag::DiagramOptions &options,
	    const Diag::Diagram::Markers &markers,
	    size_t lineIndex,
	    Diag::ShapeType::Type type);

	static const Diag::Marker *getPrev(const Diag::Marker &marker,
	    const Diag::Diagram::Markers &markers,
	    size_t lineIndex);

protected:
	size_t lineIndex;
};

}
}

#endif
