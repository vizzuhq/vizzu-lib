#ifndef GEOM_POLYGON
#define GEOM_POLYGON

#include <vector>

#include "base/geom/point.h"
#include "base/geom/rect.h"

namespace Geom
{

class Polygon
{
public:
	std::vector<Geom::Point> points;

	void add(const Geom::Point &point);
	Geom::Rect boundary() const;
};

}

#endif
