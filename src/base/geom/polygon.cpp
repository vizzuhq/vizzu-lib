#include "polygon.h"

using namespace Geom;

void Polygon::add(const Point &point) { points.push_back(point); }

Rect Polygon::boundary() const
{
	return Geom::Rect::Boundary(points);
}
