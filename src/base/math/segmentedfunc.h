#ifndef MATH_SEGMENTEDFUNC
#define MATH_SEGMENTEDFUNC

#include <vector>

namespace Math
{

template <typename T> struct SegmentedFunction
{
	struct Stop
	{
		double pos{};
		T value{};
		bool operator==(const Stop &other) const
		{
			return pos == other.pos && value == other.value;
		}
	};

	std::vector<Stop> stops;

	SegmentedFunction() noexcept = default;
	explicit SegmentedFunction(std::size_t init) : stops(init) {}
	explicit SegmentedFunction(std::vector<Stop> &&stops) noexcept :
	    stops(std::move(stops))
	{}

	SegmentedFunction operator*(double value) const;
	SegmentedFunction operator+(const SegmentedFunction &other) const;
	bool operator==(const SegmentedFunction &other) const;

	[[nodiscard]] T at(double pos) const;
};

}

#endif
