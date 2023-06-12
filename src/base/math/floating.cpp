
#include "floating.h"

#include <cmath>

using namespace Math;

int Floating::orderOfMagnitude(double base) const
{
	return (int)floor(log(std::abs(value)) / log(base));
}

bool Floating::isInteger() const { return value == floor(value); }

double Floating::sign() const { return value < 0 ? -1 : 1; }
