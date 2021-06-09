#ifndef LINEITEM_H
#define LINEITEM_H

#include "connectingitem.h"

namespace Vizzu
{
namespace Draw
{

class LineItem : public ConnectingDrawItem
{
public:
	LineItem(const Diag::Marker &item,
	    const Diag::DiagramOptions &options,
	    const Styles::Chart &style,
	    const Diag::Diagram::Markers &items,
	    size_t lineIndex);
	bool bounds(const Geom::Point &p) override;
private:
	double minWidth;
	double width;
	double maxWidth;
	double getWidth(double sizeFactor) const;
};

}
}

#endif
