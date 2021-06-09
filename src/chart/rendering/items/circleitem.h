#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include "drawitem.h"

namespace Vizzu
{
namespace Draw
{

class CircleItem : public SingleDrawItem
{
public:
	CircleItem(const Diag::Marker &item,
			const Diag::DiagramOptions &options, const Styles::Chart &style);
	bool bounds(const Geom::Point &p) override;
};

}
}

#endif
