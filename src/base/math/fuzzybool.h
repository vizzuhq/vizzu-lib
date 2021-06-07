#ifndef MATH_FUZZYBOOL
#define MATH_FUZZYBOOL

#include <stdexcept>
#include <string>
#include <functional>

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

	explicit operator bool() const {
		if (value == 1.0) return true;
		if (value == 0.0) return false;
		throw std::logic_error("internal error: fuzzy bool value");
	}

	explicit operator double() const {
		return value;
	}

	const FuzzyBool &operator=(bool value) {
		this->value = value ? 1.0 : 0.0;
		return *this;
	}

	FuzzyBool operator*(double factor) const {
		return FuzzyBool(value * factor);
	}

	FuzzyBool operator*(const FuzzyBool &v) const {
		return FuzzyBool(value * v.value);
	}

	FuzzyBool operator+(const FuzzyBool &v) const {
		return FuzzyBool(value + v.value);
	}

	void operator+=(const FuzzyBool &v) {
		*this = FuzzyBool(*this + v);
	}

	bool operator==(const FuzzyBool &v) const {
		return value == v.value;
	}

	bool operator==(bool v) const {
		return *this == FuzzyBool(v);
	}

	FuzzyBool operator!() const {
		return FuzzyBool(1.0 - value);
	}

	FuzzyBool operator&&(const FuzzyBool &v) const {
		return FuzzyBool(value * v.value);
	}

	FuzzyBool operator||(const FuzzyBool &v) const {
		return FuzzyBool(1 - (1-value) * (1-v.value));
	}

	std::string toString() const {
		if (value == 1.0) return "true";
		if (value == 0.0) return "false";
		return std::to_string(value);
	}

	void visit(const std::function<void(bool, double)> &branch) const {
		if (value > 0) branch(true, value);
		if (1 - value > 0) branch(false, 1 - value);
	}

private:
	explicit FuzzyBool(double val) {
		if (val >= 1.0 - tolerance)
			value = 1.0;
		else if (val <= 0.0 + tolerance)
			value = 0.0;
		else
			value = val;
	}

	double value;
};

}

#endif
