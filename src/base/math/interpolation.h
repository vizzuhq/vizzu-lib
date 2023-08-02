#ifndef MATH_INTERPOLATION
#define MATH_INTERPOLATION

namespace Math
{

template <typename T>
auto interpolate(const T &op0, const T &op1, double factor)
	-> decltype(op0 * (1.0 - factor) + op1 * factor)
{
	if (factor <= 0.0)
		return op0;
	if (factor >= 1.0)
		return op1;
	return op0 * (1.0 - factor) + op1 * factor;
}

}

#endif
