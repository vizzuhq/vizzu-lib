#include "base/gfx/draw/infobubble.h"

using namespace Gfx::Draw;

InfoBubble::InfoBubble(
	ICanvas &canvas, const Geom::Rect &rect, double radius,
	double arrowWidth, const Geom::Point& arrowhead)
	:
    canvas(canvas), radius(radius)
{
	typedef Geom::Point P;

	P base;
	double offset = arrowWidth / 2;

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

void InfoBubble::corner(
	const Geom::Point corner,
    const Geom::Point &dir0,
    const Geom::Point &dir1)
{
	const double kappa = 0.5522847498;
	// (4.0/3.0)*tan(M_PI/(2.0*n)), n = 4
	double distance = (1.0 - kappa) * radius;

	canvas.addPoint(corner + dir0 * radius);
	canvas.addBezier(corner + dir0 * distance,
		corner + dir1 * distance,
		corner + dir1 * radius);
}
