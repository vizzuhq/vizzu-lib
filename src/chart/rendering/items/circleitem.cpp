#include "circleitem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

CircleItem::CircleItem(const Diag::Marker &item,
    const Diag::DiagramOptions &options,
    const Styles::Chart &style) :
    SingleDrawItem(item, options, Diag::ShapeType::Circle)
{
	morphToCircle = true;
	border = false;
	auto spacing = item.spacing * item.size / 2;
	auto pos = item.position - spacing;
	auto r = *style.data.circleMaxRadius * sqrt(std::max(0.0,item.sizeFactor));
	r = std::max(r, 0.0000000001); // todo: config
	points[0] = pos + Geom::Point(-r, -r);
	points[1] = pos + Geom::Point(+r, -r);
	points[2] = pos + Geom::Point(+r, +r);
	points[3] = pos + Geom::Point(-r, +r);
	center = pos;
	lineWidth[0] = lineWidth[1] = 0;
}

bool CircleItem::bounds(const Geom::Point &p)
{
	if (!enabled) return false;
	return Geom::Circle(Geom::Rect::Boundary(points),
	    Geom::Circle::FromRect::sameWidth)
	    .contains(p);
}

