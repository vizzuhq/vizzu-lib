
#include "floating.h"

#include <cmath>

int Math::Floating::orderOfMagnitude(double base) const
{
	return static_cast<int>(floor(log(std::abs(value)) / log(base)));
}

bool Math::Floating::isInteger() const
{
	return value == floor(value);
}

double Math::Floating::sign() const { return value < 0 ? -1 : 1; }
