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
		return !is_lt(std::weak_order(value, min))
		    && !is_lt(std::weak_order(max, value));
	}

	[[nodiscard]] T rescale(const T &value, T def = T{0.5}) const
	{
		auto s = size();
		return is_zero(s) ? def : (value - min) / s;
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
		return is_zero(max) ? T{} : value / max;
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

	[[nodiscard]] Range operator*(const Transform &transf) const
	{
		return *this * transf.factor + transf.shift;
	}

	[[nodiscard]] Transform operator/(const Range range) const
	{
		auto factor =
		    is_zero(range.size()) ? T{} : size() / range.size();
		auto shift = min - range.min * factor;
		return Transform{factor, shift};
	}

	[[nodiscard]] T middle() const { return (min + max) / 2; }

	[[nodiscard]] T size() const { return max - min; }

	[[nodiscard]] bool intersects(const Range<> &range) const
	{
		using Floating::is_zero;
		auto first = std::strong_order(max, range.min);
		auto second = std::strong_order(range.max, min);

		auto isOutside =
		    is_lt(first) || is_lt(second)
		    || ((is_eq(first) || is_eq(second)) && !is_zero(size())
		        && !is_zero(range.size()));

		return !isOutside;
	}

	[[nodiscard]] Range positive() const
	{
		auto &&[min, max] = std::minmax(this->min, this->max, less);
		return {min, max};
	}

	T min{std::numeric_limits<T>::max()};
	T max{std::numeric_limits<T>::lowest()};
};

}

#endif
