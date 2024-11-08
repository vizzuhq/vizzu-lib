#ifndef MATH_RANGE
#define MATH_RANGE

#include <algorithm>
#include <cmath>
#include <limits>

#include "floating.h"

namespace Math
{

template <std::floating_point T> struct Range
{
	struct Transform
	{
		T factor;
		T shift;
	};

	constexpr static auto less = Floating::less;
	constexpr static auto is_zero = Floating::is_zero;

	constexpr static Range<T> Raw(const T &min, const T &max)
	{
		Range<T> range;
		range.min = min;
		range.max = max;
		return range;
	}

	constexpr Range() :
	    min(std::numeric_limits<T>::max()),
	    max(std::numeric_limits<T>::lowest())
	{}

	Range(const T &x, const T &y) :
	    min(std::min(x, y, less)),
	    max(std::max(x, y, less))
	{}

	[[nodiscard]] bool isReal() const
	{
		return min != std::numeric_limits<T>::max()
		    && max != std::numeric_limits<T>::lowest();
	}

	void include(const T &value)
	{
		if (!std::isfinite(value)) return;
		max = std::max(max, value, less);
		min = std::min(min, value, less);
	}

	void include(const Range<T> &range)
	{
		include(range.min);
		include(range.max);
	}

	[[nodiscard]] bool includes(const T &value) const
	{
		return !less(value, min) && !less(max, value);
	}

	[[nodiscard]] bool includes(const Range<T> &range) const
	{
		return !less(range.max, min) && !less(max, range.min);
	}

	[[nodiscard]] T rescale(const T &value) const
	{
		auto s = size();
		return is_zero(s) ? 0.5 : (value - min) / s;
	}

	[[nodiscard]] T scale(const T &value) const
	{
		return value * size() + min;
	}

	[[nodiscard]] Range<T> scale(const Range<T> &range) const
	{
		return Range<T>(scale(range.min), scale(range.max));
	}

	[[nodiscard]] T normalize(const T &value) const
	{
		return is_zero(max) ? 0 : value / max;
	}

	[[nodiscard]] Range<T> normalize(const Range<T> &range) const
	{
		return Range<T>(normalize(range.min), normalize(range.max));
	}

	bool operator==(const Range<T> &other) const
	{
		return min == other.min && max == other.max;
	}

	Range<T> operator+(double shift) const
	{
		return Range<T>(min + shift, max + shift);
	}

	Range<T> operator+(const Range<T> &other) const
	{
		return Range<T>(min + other.min, max + other.max);
	}

	Range<T> operator-(double shift) const
	{
		return Range<T>(min - shift, max - shift);
	}

	Range<T> operator*(double factor) const
	{
		return Range<T>(min * factor, max * factor);
	}

	Range<T> operator/(double factor) const
	{
		return Range<T>(min / factor, max / factor);
	}

	Range<T> operator*(const Transform &transf)
	{
		return *this * transf.factor + transf.shift;
	}

	Transform operator/(const Range<T> range)
	{
		auto factor = range.size() != 0 ? size() / range.size() : 0;
		auto shift = min - range.min * factor;
		return Transform{factor, shift};
	}

	[[nodiscard]] T middle() const { return (min + max) / 2; }

	[[nodiscard]] T size() const { return max - min; }

	[[nodiscard]] T getMin() const { return min; }
	[[nodiscard]] T getMax() const { return max; }

	consteval static auto members()
	{
		return std::tuple{&Range::min, &Range::max};
	}

protected:
	T min;
	T max;
};

}

#endif
