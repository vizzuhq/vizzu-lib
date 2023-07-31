#ifndef BASE_GEOM_BEZIER_H
#define BASE_GEOM_BEZIER_H

#include <array>

namespace Geom
{

template <typename T> class CubicBezier
{
public:
	CubicBezier(T val0, T val1, T val2, T val3) :
	    values{val0, val1, val2, val3}
	{}

	T operator()(double t) const
	{
		auto it = 1 - t;

		return values[0] * it * it * it + values[1] * 3 * it * it * t
		     + values[2] * 3 * it * t * t + values[3] * t * t * t;
	}

private:
	std::array<T, 4> values;
};

}

#endif
