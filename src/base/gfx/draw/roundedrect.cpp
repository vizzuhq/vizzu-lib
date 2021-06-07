#include "base/gfx/draw/roundedrect.h"

using namespace Gfx::Draw;

RoundedRect::RoundedRect(
	ICanvas &canvas,
    const Geom::Rect &rect,
    double radius) :
    canvas(canvas),
    radius(radius)
{
	typedef Geom::Point P;

	canvas.beginPolygon();
	canvas.addPoint(rect.bottomLeft() + P::X(radius));

	corner(rect.bottomRight(), P::X(-1), P::Y(1));
	corner(rect.topRight(), P::Y(-1), P::X(-1));
	corner(rect.topLeft(), P::X(1), P::Y(-1));
	corner(rect.bottomLeft(), P::Y(1), P::X(1));

	canvas.endPolygon();
}

void RoundedRect::corner(
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
