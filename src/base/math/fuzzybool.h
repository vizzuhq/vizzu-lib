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
	static constexpr double tolerance = 0.000000001;
	FuzzyBool() = default;

	static FuzzyBool True() { return FuzzyBool(true); }
	static FuzzyBool False() { return FuzzyBool(false); }

	explicit FuzzyBool(bool value) : value(value ? 1.0 : 0.0) {}

	template <class Optional>
	    requires(std::is_convertible_v<bool, Optional>
	             && std::is_same_v<
	                 std::remove_cvref_t<
	                     decltype(*std::declval<Optional>())>,
	                 bool>)
	// NOLINTNEXTLINE(bugprone-forwarding-reference-overload)
	explicit FuzzyBool(Optional &&opt) :
	    FuzzyBool(opt && *std::forward<Optional>(opt))
	{}

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

	FuzzyBool &operator=(bool value)
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

	void operator+=(const FuzzyBool &v) { *this = *this + v; }

	bool operator==(const FuzzyBool &v) const
	{
		return value == v.value;
	}

	bool operator!=(const FuzzyBool &v) const
	{
		return value != v.value;
	}

	bool operator==(bool v) const { return *this == FuzzyBool(v); }

	bool operator!=(bool v) const { return *this != FuzzyBool(v); }

	friend bool operator==(bool b, const FuzzyBool &v)
	{
		return v == FuzzyBool(b);
	}

	friend bool operator!=(bool b, const FuzzyBool &v)
	{
		return v != FuzzyBool(b);
	}

	FuzzyBool operator!() const { return FuzzyBool(1.0 - value); }

	FuzzyBool operator&&(const FuzzyBool &v) const
	{
		return FuzzyBool(std::min(value, v.value));
	}

	FuzzyBool operator||(const FuzzyBool &v) const
	{
		return FuzzyBool(std::max(value, v.value));
	}

	[[nodiscard]] std::string toString() const
	{
		if (value == 1.0) return "true";
		if (value == 0.0) return "false";
		return std::to_string(value);
	}

	[[nodiscard]] FuzzyBool more() const
	{
		return FuzzyBool(std::max(0.0, 2 * value - 1));
	}

	[[nodiscard]] FuzzyBool less() const
	{
		return FuzzyBool(std::max(0.0, 1 - 2 * value));
	}

	[[nodiscard]] FuzzyBool very() const
	{
		return FuzzyBool(value * value);
	}

	[[nodiscard]] FuzzyBool somewhat() const
	{
		return FuzzyBool(sqrt(value));
	}

private:
	double value{0.0};
};

FuzzyBool operator&&(double, const FuzzyBool &v) = delete;
FuzzyBool operator&&(const FuzzyBool &v, double) = delete;
FuzzyBool operator||(double, const FuzzyBool &v) = delete;
FuzzyBool operator||(const FuzzyBool &v, double) = delete;

inline bool operator&&(bool b, const FuzzyBool &v)
{
	return b && static_cast<bool>(v);
}
inline bool operator&&(const FuzzyBool &v, bool b)
{
	return b && static_cast<bool>(v);
}
inline bool operator||(bool b, const FuzzyBool &v)
{
	return b || static_cast<bool>(v);
}
inline bool operator||(const FuzzyBool &v, bool b)
{
	return b || static_cast<bool>(v);
}
}

#endif
