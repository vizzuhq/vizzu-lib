#ifndef MATH_FLOATING
#define MATH_FLOATING

namespace Math
{

class Floating
{
public:
	explicit Floating(double value) : value(value) {}

	[[nodiscard]] int orderOfMagnitude(double base = 10) const;
	[[nodiscard]] bool isInteger() const;
	[[nodiscard]] double sign() const;

private:
	double value;
};

}

#endif
