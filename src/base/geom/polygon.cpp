#include "polygon.h"

#include "point.h"
#include "rect.h"

namespace Geom
{

void Polygon::add(const Point &point) { points.push_back(point); }

Rect Polygon::boundary() const { return Rect::Boundary(points); }

}