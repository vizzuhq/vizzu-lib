#ifndef MATH_SEGMENTEDFUNC
#define MATH_SEGMENTEDFUNC

#include <vector>

namespace Math
{

template <typename T> struct SegmentedFunction
{
	struct Stop
	{
		double pos;
		T value;
		Stop() : pos(0), value(T()) {}
		Stop(double pos, T value) : pos(pos), value(value) {}
		bool operator==(const Stop &other) const
		{
			return pos == other.pos && value == other.value;
		}
	};

	std::vector<Stop> stops;

	SegmentedFunction() = default;
	SegmentedFunction(std::vector<Stop> stops) :
	    stops(std::move(stops))
	{}

	SegmentedFunction operator*(double value) const;
	SegmentedFunction operator+(const SegmentedFunction &other) const;
	bool operator==(const SegmentedFunction &other) const;

	[[nodiscard]] T at(double pos) const;
};

}

#endif
