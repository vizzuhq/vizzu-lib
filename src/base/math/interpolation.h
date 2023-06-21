#ifndef MATH_INTERPOLATION
#define MATH_INTERPOLATION

namespace Math
{

template <typename T>
T interpolate(const T &op0, const T &op1, double factor)
{
	if (factor <= 0.0)
		return op0;
	else if (factor >= 1.0)
		return op1;
	else
		return op0 * (1.0 - factor) + op1 * factor;
}

}

#endif
