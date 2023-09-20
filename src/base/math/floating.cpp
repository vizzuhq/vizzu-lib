
#include "floating.h"

#include <cmath>

namespace Math
{

int Floating::orderOfMagnitude(double base) const
{
	return static_cast<int>(floor(log(std::abs(value)) / log(base)));
}

bool Floating::isInteger() const { return value == floor(value); }

double Floating::sign() const { return value < 0 ? -1 : 1; }

}