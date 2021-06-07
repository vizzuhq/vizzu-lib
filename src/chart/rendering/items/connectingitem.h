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
	ConnectingDrawItem(const Diag::DiagramItem &item,
					   const Diag::DiagramOptions &options,
					   const Diag::Diagram::Items &items,
					   size_t lineIndex,
					   Diag::ShapeType::Type type);

	static const Diag::DiagramItem *getPrev(const Diag::DiagramItem &item,
			const Diag::Diagram::Items &items, size_t lineIndex);
protected:
	size_t lineIndex;
};

}
}

#endif
