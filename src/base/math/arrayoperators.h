#ifndef MATH_ARRAYOPERATORS
#define MATH_ARRAYOPERATORS

#include <array>
#include <cstddef>

namespace Math
{

template <typename T, size_t N>
std::array<T, N> operator*(const std::array<T, N> &array,
    double factor)
{
	std::array<T, N> res;
	for (auto i = 0U; i < N; i++) res[i] = array[i] * factor;
	return res;
}

template <typename T, size_t N>
std::array<T, N> operator+(const std::array<T, N> &array0,
    const std::array<T, N> &array1)
{
	std::array<T, N> res;
	for (auto i = 0U; i < N; i++) res[i] = array0[i] + array1[i];
	return res;
}

}

#endif
