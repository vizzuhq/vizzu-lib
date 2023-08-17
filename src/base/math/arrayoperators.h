#ifndef MATH_ARRAYOPERATORS
#define MATH_ARRAYOPERATORS

#include <array>
#include <cstddef>
#include <functional>
#include <ranges>

namespace Math
{

template <typename T, size_t N>
std::array<T, N> operator*(const std::array<T, N> &array,
    double factor)
{
	return []<std::size_t... Ix>(const std::array<T, N> &array,
	           double factor,
	           std::index_sequence<Ix...>)
	{
		return std::array<T, N>{std::get<Ix>(array) * factor...};
	}(array, factor, std::make_index_sequence<N>{});
}

template <typename T, size_t N>
std::array<T, N> operator+(const std::array<T, N> &array0,
    const std::array<T, N> &array1)
{
	return []<std::size_t... Ix>(const std::array<T, N> &array0,
	           const std::array<T, N> &array1,
	           std::index_sequence<Ix...>)
	{
		return std::array<T, N>{
		    (std::get<Ix>(array0) + std::get<Ix>(array1))...};
	}(array0, array1, std::make_index_sequence<N>{});
}

}

#endif
