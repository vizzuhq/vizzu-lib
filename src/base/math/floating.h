#ifndef MATH_FLOATING
#define MATH_FLOATING

#include <compare>
#include <concepts>
#include <limits>
#include <type_traits>

namespace Math::Floating
{

constexpr auto inline less =
    []<std::floating_point T>(T a, std::type_identity_t<T> b)
{
	return std::is_lt(std::strong_order(a, b));
};

constexpr auto inline is_zero = []<std::floating_point T>(T value)
{
	if constexpr (std::numeric_limits<T>::is_iec559)
		return value == T{};
	else
		return std::is_eq(std::weak_order(T{}, value));
};

}

#endif
