#ifndef GEOM_TRIANGLE
#define GEOM_TRIANGLE

#include <array>

#include "point.h"

namespace Geom
{

class Triangle
{
public:
	std::array<Point, 3> points;

	[[nodiscard]] double area() const;
};

}

#endif
