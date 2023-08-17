#include "normalizednumber.h"

#include <algorithm>
#include <cmath>

#include "floating.h"

using namespace Math;

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
		auto sign = Floating(value).sign();
		value *= sign;
		positive = sign == 1;
		exponent = Floating(value).orderOfMagnitude(base);
		coefficient = value / pow(base, exponent);
		coefficient = std::min(std::max(1.0, coefficient), base);
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
