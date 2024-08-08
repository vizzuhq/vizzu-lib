#ifndef MATH_TOLERANCE
#define MATH_TOLERANCE

#include <cmath>

namespace Math
{

struct AddTolerance
{
	double value;
	double tolerance;

	explicit AddTolerance(double value, double tolerance = 0.00001) :
	    value(value),
	    tolerance(tolerance)
	{}

	bool operator<(double other) const
	{
		return value < other * (1 - tolerance);
	}

	bool operator>(double other) const
	{
		return value > other * (1 + tolerance);
	}

	bool operator<=(double other) const { return !(*this > other); }

	bool operator>=(double other) const { return !(*this < other); }

	bool operator==(double other) const
	{
		return !(*this < other || *this > other);
	}
};

}

#endif
