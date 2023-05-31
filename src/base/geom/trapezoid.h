#ifndef GEOM_TRAPEZOID
#define GEOM_TRAPEZOID

#include <array>

#include "quadrilateral.h"

namespace Geom
{

class VerticalTrapezoid : public ComplexQuadrilateral
{
public:
	using ComplexQuadrilateral::ComplexQuadrilateral;
	bool contains(const Point &p) const;
};

}

#endif
