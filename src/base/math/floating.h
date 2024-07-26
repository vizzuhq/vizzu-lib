#ifndef MATH_FLOATING
#define MATH_FLOATING

#include <compare>
#include <concepts>
#include <limits>

namespace Math::Floating
{

[[nodiscard]] int orderOfMagnitude(double value, double base = 10);

constexpr auto inline less = [](auto a, auto b)
{
	static_assert(std::floating_point<decltype(a)>);
	return std::is_lt(std::strong_order(a, b));
};

constexpr auto inline is_zero = [](auto value)
{
	using F = decltype(value);
	static_assert(std::floating_point<F>);
	if constexpr (std::numeric_limits<F>::is_iec559) {
		return value == F{};
	}
	else {
		return std::is_eq(std::weak_order(F{}, value));
	}
};

}

#endif
