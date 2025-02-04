#ifndef MATH_SEGMENTEDFUNC
#define MATH_SEGMENTEDFUNC

#include <initializer_list>
#include <vector>

#include "base/alg/merge.h"

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

		auto merger = [](const Stop &item1, const Stop &item2)
		{
			return Stop{item1.pos, item1.value + item2.value};
		};

		auto &&transformer = [](const CRTP &another)
		{
			return [&another](const Stop &item)
			{
				return Stop{item.pos, item.value + another(item.pos)};
			};
		};
		Alg::merge(self.stops,
		    other.stops,
		    res.stops,
		    Alg::merge_args
		    // { Remove when clang-16 not used
		    <std::identity,
		        std::identity,
		        double Stop::*,
		        decltype(std::weak_order),
		        decltype(transformer(other)),
		        decltype(transformer(self)),
		        Alg::Merge::always,
		        decltype(merger)>
		    // }
		    {.projection = &Stop::pos,
		        .transformer_1 = transformer(other),
		        .transformer_2 = transformer(self),
		        .merger = merger});
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
