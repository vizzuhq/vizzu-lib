#ifndef MATH_FUZZYBOOL
#define MATH_FUZZYBOOL

#include <cmath>
#include <functional>
#include <stdexcept>
#include <string>

namespace Math
{

class FuzzyBool
{
public:
	typedef double UnderlyingType;
	static constexpr double tolerance = 0.000000001;
	FuzzyBool() : value(0.0) {}

	static FuzzyBool True() { return FuzzyBool(true); }
	static FuzzyBool False() { return FuzzyBool(false); }

	explicit FuzzyBool(bool value) : value(value ? 1.0 : 0.0) {}

	explicit FuzzyBool(double val)
	{
		if (val >= 1.0 - tolerance)
			value = 1.0;
		else if (val <= 0.0 + tolerance)
			value = 0.0;
		else
			value = val;
	}

	explicit operator bool() const
	{
		if (value == 1.0) return true;
		if (value == 0.0) return false;
		throw std::logic_error("internal error: fuzzy bool value");
	}

	explicit operator double() const { return value; }

	const FuzzyBool &operator=(bool value)
	{
		this->value = value ? 1.0 : 0.0;
		return *this;
	}

	FuzzyBool operator*(double factor) const
	{
		return FuzzyBool(value * factor);
	}

	FuzzyBool operator+(const FuzzyBool &v) const
	{
		return FuzzyBool(value + v.value);
	}

	void operator+=(const FuzzyBool &v)
	{
		*this = FuzzyBool(*this + v);
	}

	bool operator==(const FuzzyBool &v) const
	{
		return value == v.value;
	}

	bool operator==(bool v) const { return *this == FuzzyBool(v); }

	FuzzyBool operator!() const { return FuzzyBool(1.0 - value); }

	FuzzyBool operator&&(const FuzzyBool &v) const
	{
		return FuzzyBool(std::min(value, v.value));
	}

	FuzzyBool operator||(const FuzzyBool &v) const
	{
		return FuzzyBool(std::max(value, v.value));
	}

	std::string toString() const
	{
		if (value == 1.0) return "true";
		if (value == 0.0) return "false";
		return std::to_string(value);
	}

	void visit(const std::function<void(bool, double)> &branch) const
	{
		if (value > 0) branch(true, value);
		if (1 - value > 0) branch(false, 1 - value);
	}

	FuzzyBool more() const
	{
		return FuzzyBool(std::max(0.0, 2 * value - 1));
	}

	FuzzyBool less() const
	{
		return FuzzyBool(std::max(0.0, 1 - 2 * value));
	}

	FuzzyBool very() const { return FuzzyBool(value * value); }

	FuzzyBool somewhat() const { return FuzzyBool(sqrt(value)); }

private:
	double value;
};

}

#endif
