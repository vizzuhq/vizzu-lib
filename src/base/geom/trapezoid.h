#ifndef GEOM_TRAPEZOID
#define GEOM_TRAPEZOID

#include <array>

#include "quadrilateral.h"

namespace Geom
{

class VerticalTrapezoid : public ConvexQuad
{
public:
	using ConvexQuad::ConvexQuad;
	bool contains(const Point &p) const;
};

}

#endif
