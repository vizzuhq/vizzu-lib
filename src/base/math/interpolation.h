#ifndef MATH_INTERPOLATION
#define MATH_INTERPOLATION

#include <type_traits>

#include "base/refl/auto_struct.h"

namespace Math
{

template <typename T>
    requires(!std::is_same_v<std::remove_cvref_t<T>, bool>)
auto interpolate(const T &op0, const T &op1, double factor)
    -> decltype(op0 * (1.0 - factor) + op1 * factor)
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
			} -> std::same_as<T>;
		};
	}
};

template <class T> concept interpolatable = interpolatable_t<T>{}();

struct interpolate_t
{
	template <class T>
	constexpr T
	operator()(const T &a, const T &b, double factor) const
	{
		using Math::interpolate;
		if constexpr (interpolatable<T>)
			return interpolate(a, b, factor);
		else {
			T res;
			Refl::visit(
			    [factor]<interpolatable V>(V &res,
			        const V &op0,
			        const V &op1)
			    {
				    res = interpolate(op0, op1, factor);
			    },
			    res,
			    a,
			    b);
			return res;
		}
	}
};

constexpr inline static interpolate_t interpolate{};
}

}

#endif
