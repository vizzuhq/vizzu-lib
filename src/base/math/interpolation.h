#ifndef MATH_INTERPOLATION
#define MATH_INTERPOLATION

#include <type_traits>

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

}

#endif
