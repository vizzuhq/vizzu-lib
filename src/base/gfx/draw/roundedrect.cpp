#include "base/gfx/draw/roundedrect.h"

#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

namespace Gfx::Draw
{

RoundedRect::RoundedRect(ICanvas &canvas,
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

void RoundedRect::corner(const Geom::Point corner,
    const Geom::Point &dir0,
    const Geom::Point &dir1)
{
	constexpr double tan_piPer8 = 0.41421356237;
	constexpr double kappa = (4.0 / 3.0) * tan_piPer8;
	auto distance = (1.0 - kappa) * radius;

	canvas.addPoint(corner + dir0 * radius);
	canvas.addBezier(corner + dir0 * distance,
	    corner + dir1 * distance,
	    corner + dir1 * radius);
}

}