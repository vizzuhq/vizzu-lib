#ifndef MATH_RANGE
#define MATH_RANGE

#include <limits>
#include <algorithm>
#include <string>

namespace Math
{

template <typename T>
struct Range
{
public:
	struct Transform {
		double factor;
		double shift;
	};

	Range()
		: min(std::numeric_limits<T>::max()),
		  max(std::numeric_limits<T>::lowest())
	{}

	Range(const T &x, const T &y)
		: min(std::min(x, y)),
		  max(std::max(x, y))
	{}

	bool isReal() const {
		return min != std::numeric_limits<T>::max()
			&& max != std::numeric_limits<T>::lowest();
	}

	bool isNull() const {
		return min == 0.0 && max == 0.0;
	}

	void include(const T &value) {
		max = std::max(max, value);
		min = std::min(min, value);
	}

	void include(const Range<T> &range) {
		include(range.min);
		include(range.max);
	}

	bool includes(const T &value) const {
		return value >= min && value <= max;
	}

	T rescale(const T &value) const {
		if (max == min) return 0.5;
		else return (value - min) / size();
	}

	Range<T> rescale(const Range<T> &range) const {
		return Range<T>(rescale(range.min),
						rescale(range.max));
	}

	T scale(const T &value) const {
		return value * size() + min;
	}

	Range<T> scale(const Range<T> &range) const {
		return Range<T>(scale(range.min),
						scale(range.max));
	}

	T normalize(const T &value) const {
		if (max == 0) return 0;
		else return value / max;
	}

	Range<T> normalize(const Range<T> &range) const {
		return Range<T>(normalize(range.min),
						normalize(range.max));
	}

	bool operator==(const Range<T> &other) const {
		return min == other.min && max == other.max;
	}

	Range<T> operator+(double shift) const {
		return Range<T>(min + shift, max + shift);
	}

	Range<T> operator+(const Range<T> &other) const {
		return Range<T>(min + other.min, max + other.max);
	}

	Range<T> operator-(double shift) const {
		return Range<T>(min - shift, max - shift);
	}

	Range<T> operator*(double factor) const {
		return Range<T>(min * factor, max * factor);
	}

	Range<T> operator/(double factor) const {
		return Range<T>(min / factor, max / factor);
	}

	Range<T> operator*(const Transform &transf) {
		return *this * transf.factor + transf.shift;
	}

	Transform operator/(const Range<T> range) {
		auto factor = range.size() != 0 ? size() / range.size() : 0;
		auto shift = min - range.min * factor;
		return Transform{ factor, shift };
	}

	T middle() const {
		return (min + max) / 2;
	}

	T size() const {
		return (max - min);
	}

	T getMin() const { return min; }
	T getMax() const { return max; }

	explicit operator std::string() const {
		return "{ " + std::to_string(min) + ", " + std::to_string(max) + " }";
	}

protected:
	T min;
	T max;
};

}

#endif
