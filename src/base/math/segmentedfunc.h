#ifndef MATH_SEGMENTEDFUNC
#define MATH_SEGMENTEDFUNC

#include <initializer_list>
#include <vector>

#include "interpolation.h"
#include "range.h"

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

	SegmentedFunction(std::initializer_list<Stop> il) : stops(il) {}

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
			if (it1 == other.stops.end()
			    || (it0 != stops.end() && it0->pos < it1->pos)) {
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

	[[nodiscard]] T operator()(double pos) const
	{
		if (stops.empty()) return T();
		if (stops.size() == 1 || pos < stops.front().pos)
			return stops.front().value;
		if (pos > stops.back().pos) return stops.back().value;

		auto it = std::ranges::adjacent_find(stops,
		    [pos](auto &&cur, auto &&next)
		    {
			    return cur.pos <= pos && pos <= next.pos;
		    });
		return interpolate(it->value,
		    std::next(it)->value,
		    Range<>{it->pos, std::next(it)->pos}.rescale(pos));
	}

protected:
	explicit SegmentedFunction(std::size_t init) : stops(init) {}
};

}

#endif
