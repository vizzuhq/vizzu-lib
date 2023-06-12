#ifndef MATH_STATISTICS
#define MATH_STATISTICS

#include <numeric>

namespace Math
{

template <size_t N, typename T>
static T mean(const std::array<T, N> &values)
{
	return std::accumulate(values.begin(), values.end(), T()) / N;
}

}

#endif
