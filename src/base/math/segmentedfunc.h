#ifndef MATH_SEGMENTEDFUNC
#define MATH_SEGMENTEDFUNC

#include <vector>

namespace Math
{

template <typename T, class CRTP> struct SegmentedFunction
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

	[[nodiscard]] friend CRTP operator*(const CRTP &self,
	    double value)
	{
		auto res = self;
		for (auto &stop : res.stops) stop.value = stop.value * value;
		return res;
	}

	[[nodiscard]] friend CRTP operator+(const CRTP &self,
	    const CRTP &other)
	{
		CRTP res;
		auto &stops = self.stops;

		for (auto it0 = stops.begin(), it1 = other.stops.begin();
		     it0 != stops.end() || it1 != other.stops.end();) {
			if (it1 == other.stops.end() || it0->pos < it1->pos) {
				res.stops.emplace_back(it0->pos,
				    it0->value + other(it0->pos));
				++it0;
			}
			else if (it0 == stops.end() || it1->pos < it0->pos) {
				res.stops.emplace_back(it1->pos,
				    it1->value + self(it1->pos));
				++it1;
			}
			else {
				res.stops.emplace_back(it0->pos,
				    it0->value + it1->value);
				++it1;
				++it0;
			}
		}
		return res;
	}

	[[nodiscard]] bool operator==(
	    const SegmentedFunction &other) const = default;

	[[nodiscard]] T operator()(double pos) const;
};

}

#endif
