#include "polygon.h"

void Geom::Polygon::add(const Point &point)
{
	points.push_back(point);
}

Geom::Rect Geom::Polygon::boundary() const
{
	return Geom::Rect::Boundary(points);
}
