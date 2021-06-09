#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H

#include "drawitem.h"

namespace Vizzu
{
namespace Draw
{

class RectangleItem : public SingleDrawItem
{
public:
	RectangleItem(const Diag::Marker &marker,
			const Diag::DiagramOptions &options,
			const Styles::Chart &style);
	bool bounds(const Geom::Point &p) override;
};


}
}

#endif
