#ifndef LINEITEM_H
#define LINEITEM_H

#include "connectingitem.h"
#include "chart/rendering/painter/coordinatesystem.h"

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
	    const CoordinateSystem &coordSys,
	    const Diag::Diagram::Markers &markers,
	    size_t lineIndex);
	bool bounds(const Geom::Point &p) override;
};

}
}

#endif
