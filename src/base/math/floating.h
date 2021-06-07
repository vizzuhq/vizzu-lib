#ifndef MATH_FLOATING
#define MATH_FLOATING

namespace Math
{

class Floating
{
public:

	explicit Floating(double value)
		: value(value)
	{}

	int orderOfMagnitude(double base = 10) const;
	bool isInteger() const;
	double sign() const;

private:
	double value;
};

}

#endif

