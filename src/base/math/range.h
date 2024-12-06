#ifndef MATH_RANGE
#define MATH_RANGE

#include <algorithm>
#include <cmath>
#include <limits>

#include "floating.h"

namespace Math
{

template <std::floating_point T = double> struct Range
{
	struct Transform
	{
		T factor;
		T shift;
	};

	constexpr static auto less = Floating::less;
	constexpr static auto is_zero = Floating::is_zero;

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

	void include(const Range &range)
	{
		include(range.min);
		include(range.max);
	}

	[[nodiscard]] bool includes(const T &value) const
	{
		return !less(value, min) && !less(max, value);
	}

	[[nodiscard]] bool includes(const Range &range) const
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

	[[nodiscard]] Range scale(const Range &range) const
	{
		return {scale(range.min), scale(range.max)};
	}

	[[nodiscard]] T normalize(const T &value) const
	{
		return is_zero(max) ? 0 : value / max;
	}

	[[nodiscard]] Range normalize(const Range &range) const
	{
		return {normalize(range.min), normalize(range.max)};
	}

	bool operator==(const Range &other) const
	{
		return min == other.min && max == other.max;
	}

	Range operator+(double shift) const
	{
		return {min + shift, max + shift};
	}

	Range operator+(const Range &other) const
	{
		return {min + other.min, max + other.max};
	}

	Range operator-(double shift) const
	{
		return {min - shift, max - shift};
	}

	Range operator*(double factor) const
	{
		return {min * factor, max * factor};
	}

	Range operator/(double factor) const
	{
		return {min / factor, max / factor};
	}

	Range operator*(const Transform &transf)
	{
		return *this * transf.factor + transf.shift;
	}

	Transform operator/(const Range range)
	{
		auto factor = range.size() != 0 ? size() / range.size() : 0;
		auto shift = min - range.min * factor;
		return Transform{factor, shift};
	}

	[[nodiscard]] T middle() const { return (min + max) / 2; }

	[[nodiscard]] T size() const { return max - min; }

	T min{std::numeric_limits<T>::max()};
	T max{std::numeric_limits<T>::lowest()};
};

}

#endif
