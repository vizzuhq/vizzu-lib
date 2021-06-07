#ifndef GEOM_BEZIER
#define GEOM_BEZIER

#include <array>

namespace Geom
{

template <typename T>
class QuadBezier
{
public:
	QuadBezier(T val0,
	    T val1,
	    T val2) :
	    values{val0, val1, val2}
	{}

	T operator()(double t) const
	{
		auto it = 1 - t;

		return values[0] * it * it + values[1] * 2 * it * t
		     + values[2] * t * t;
	}

private:
	std::array<T, 3> values;
};

template <typename T>
class CubicBezier
{
public:
	CubicBezier(T val0,
	    T val1,
	    T val2,
	    T val3) :
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
