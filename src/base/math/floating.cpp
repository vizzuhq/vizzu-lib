
#include "floating.h"

#include <cmath>

namespace Math::Floating
{

int orderOfMagnitude(double value, double base)
{
	return static_cast<int>(floor(log(std::abs(value)) / log(base)));
}

}