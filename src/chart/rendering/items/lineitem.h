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
	LineItem(const Diag::Marker &marker,
	    const Diag::Options &options,
	    const Styles::Chart &style,
	    const Diag::Diagram::Markers &markers,
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
