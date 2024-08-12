#include "infobubble.h"

#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

#include "roundedrect.h"

namespace Gfx::Draw
{

InfoBubble::InfoBubble(ICanvas &canvas,
    const Geom::Rect &rect,
    double radius,
    double arrowWidth,
    const Geom::Point &arrowhead) :
    RoundedRect(canvas, rect, radius)
{
	typedef Geom::Point P;

	P base;
	auto offset = arrowWidth / 2;

	canvas.beginPolygon();
	canvas.addPoint(rect.bottomLeft() + P::X(radius));

	if (rect.bottomLeft().y > arrowhead.y) {
		base.x = arrowhead.x;
		base.y = rect.bottomLeft().y;
		canvas.addPoint(base - P::X(offset));
		canvas.addPoint(arrowhead);
		canvas.addPoint(base + P::X(offset));
	}

	corner(rect.bottomRight(), P::X(-1), P::Y(1));

	if (rect.bottomRight().x < arrowhead.x) {
		base.y = arrowhead.y;
		base.x = rect.bottomRight().x;
		canvas.addPoint(base - P::Y(offset));
		canvas.addPoint(arrowhead);
		canvas.addPoint(base + P::Y(offset));
	}

	corner(rect.topRight(), P::Y(-1), P::X(-1));

	if (rect.topRight().y < arrowhead.y) {
		base.x = arrowhead.x;
		base.y = rect.topRight().y;
		canvas.addPoint(base + P::X(offset));
		canvas.addPoint(arrowhead);
		canvas.addPoint(base - P::X(offset));
	}

	corner(rect.topLeft(), P::X(1), P::Y(-1));

	if (rect.topLeft().x > arrowhead.x) {
		base.y = arrowhead.y;
		base.x = rect.topLeft().x;
		canvas.addPoint(base + P::Y(offset));
		canvas.addPoint(arrowhead);
		canvas.addPoint(base - P::Y(offset));
	}

	corner(rect.bottomLeft(), P::Y(1), P::X(1));
	canvas.endPolygon();
}

}