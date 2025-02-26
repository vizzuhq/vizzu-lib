#ifndef MATH_INTERPOLATION
#define MATH_INTERPOLATION

#include <type_traits>

#include "base/refl/auto_struct.h"

namespace Math
{

template <typename T>
    requires(!std::is_same_v<std::remove_cvref_t<T>, bool>)
auto interpolate(const T &op0,
    const T &op1,
    double factor) -> decltype(op0 * (1.0 - factor) + op1 * factor)
{
	if (factor <= 0.0) return op0;
	if (factor >= 1.0) return op1;
	return op0 * (1.0 - factor) + op1 * factor;
}

namespace Niebloid
{
template <class T> struct interpolatable_t
{
	consteval bool operator()() const
	{
		using Math::interpolate;
		return requires(const T &a, const T &b, double factor) {
			{
				interpolate(a, b, factor)
			}
			-> std::same_as<
			    std::conditional_t<std::is_integral_v<T>, double, T>>;
		};
	}
};

template <class T> concept interpolatable = interpolatable_t<T>{}();

struct interpolate_t
{
	template <class T>
	constexpr auto
	operator()(const T &a, const T &b, double factor) const
	{
		using Math::interpolate;
		if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>)
			return Math::interpolate<double>(a, b, factor);
		else if constexpr (interpolatable<T>)
			return interpolate(a, b, factor);
		else {
			return Refl::visit<T>(
			    [factor]<interpolatable V>(const V &op0, const V &op1)
			    {
				    return interpolate(op0, op1, factor);
			    },
			    a,
			    b);
		}
	}
};

constexpr inline static interpolate_t interpolate{};
}

}

#endif
