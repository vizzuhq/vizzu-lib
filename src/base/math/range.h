#ifndef MATH_RANGE
#define MATH_RANGE

#include <algorithm>
#include <cmath>
#include <limits>
#include <string>

namespace Math
{

template <typename T> struct Range
{
public:
	struct Transform
	{
		double factor;
		double shift;
	};

	static Range<T> Raw(const T &min, const T &max)
	{
		Range<T> range;
		range.min = min;
		range.max = max;
		return range;
	}

	Range() :
	    min(std::numeric_limits<T>::max()),
	    max(std::numeric_limits<T>::lowest())
	{}

	Range(const T &x, const T &y) :
	    min(std::min(x, y)),
	    max(std::max(x, y))
	{}

	[[nodiscard]] bool isReal() const
	{
		return min != std::numeric_limits<T>::max()
		    && max != std::numeric_limits<T>::lowest();
	}

	[[nodiscard]] bool isNull() const
	{
		return min == 0.0 && max == 0.0;
	}

	void include(const T &value)
	{
		if (!std::isfinite(value)) return;
		max = std::max(max, value);
		min = std::min(min, value);
	}

	void include(const Range<T> &range)
	{
		include(range.min);
		include(range.max);
	}

	[[nodiscard]] bool includes(const T &value) const
	{
		return value >= min && value <= max;
	}

	[[nodiscard]] bool includes(const Range<T> &range) const
	{
		return range.max >= min && range.min <= max;
	}

	[[nodiscard]] T rescale(const T &value) const
	{
		return max == min ? 0.5 : (value - min) / size();
	}

	[[nodiscard]] Range<T> rescale(const Range<T> &range) const
	{
		return Range<T>(rescale(range.min), rescale(range.max));
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
		return max == 0 ? 0 : value / max;
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

	[[nodiscard]] T size() const { return (max - min); }

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
