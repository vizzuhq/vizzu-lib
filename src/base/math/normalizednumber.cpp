#include "normalizednumber.h"

#include <algorithm>
#include <cmath>

#include "floating.h"

namespace Math
{

NormalizedNumber::NormalizedNumber(bool positive,
    double coefficient,
    int exponent,
    double base) :
    positive(positive),
    coefficient(coefficient),
    exponent(exponent),
    base(base)
{}

NormalizedNumber::NormalizedNumber(double value, double base) :
    base(base)
{
	if (value != 0) {
		positive = !std::signbit(value);
		if (!positive) value = -value;
		exponent = static_cast<int>(floor(log(value) / log(base)));
		coefficient = value / pow(base, exponent);
		coefficient = std::clamp(coefficient, 1.0, base);
	}
	else {
		positive = true;
		coefficient = 0;
		exponent = 0;
	}
}

double NormalizedNumber::value() const
{
	return (positive ? 1 : -1) * coefficient * pow(base, exponent);
}

void NormalizedNumber::setExponent(int exp)
{
	coefficient *= pow(base, exponent - exp);
	exponent = exp;
}

}