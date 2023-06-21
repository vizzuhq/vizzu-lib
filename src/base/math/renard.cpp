
#include "renard.h"

#include <cmath>
#include <stdexcept>
#include <ranges>

#include "floating.h"
#include "normalizednumber.h"

using namespace Math;

const std::vector<double> Renard::R3Numbers = {1.0, 2.0, 5.0, 10.0};

const std::vector<double> Renard::R5Numbers =
    {1.0, 1.5, 2.5, 4.0, 6.0, 10.0};

Renard Renard::R3() { return Renard(R3Numbers); }
Renard Renard::R5() { return Renard(R5Numbers); }

double Renard::ceil(double value)
{
	if (value == 0.0) return 0.0;

	ScientificNumber num(value);

	for (double Rnum : numbers)
		if (num.coefficient <= Rnum)
			return ScientificNumber(num.positive, Rnum, num.exponent)
			    .value();

	throw std::logic_error("Internal error in R-number rounding.");
}

double Renard::floor(double value)
{
	if (value == 0.0) return 0.0;

	ScientificNumber num(value);

	for (auto Rnum : std::ranges::reverse_view(numbers))
		if (num.coefficient >= Rnum)
			return ScientificNumber(num.positive, Rnum, num.exponent)
			    .value();

	throw std::logic_error("Internal error in R-number rounding.");
}
