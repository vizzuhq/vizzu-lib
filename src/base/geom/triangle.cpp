#include "triangle.h"

#include <cmath>

namespace Geom
{

double Triangle::area() const
{
	auto A = points[2] - points[1];
	auto B = points[0] - points[1];
	return fabs(A ^ B) / 2;
}

}