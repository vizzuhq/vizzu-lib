#ifndef MATH_SEGMENTEDFUNC_TPP
#define MATH_SEGMENTEDFUNC_TPP

#include <ranges>

#include "base/math/interpolation.h"
#include "base/math/range.h"

#include "segmentedfunc.h"

template <typename T>
T Math::SegmentedFunction<T>::operator()(double pos) const
{
	if (stops.empty()) return T();
	if (stops.size() == 1 || pos < stops.front().pos)
		return stops.front().value;

	if (auto it = std::ranges::adjacent_find(stops,
	        [pos](auto &&cur, auto &&next)
	        {
		        return cur.pos <= pos && pos <= next.pos;
	        });
	    it != stops.end())
		return interpolate(it->value,
		    std::next(it)->value,
		    Range{it->pos, std::next(it)->pos}.rescale(pos));

	return stops.back().value;
}

template <typename T>
template <class Self>
Self Math::SegmentedFunction<T>::operator*(this const Self &self,
    double value)
{
	auto res = self;
	for (auto &stop : res.stops) stop.value = stop.value * value;
	return res;
}

template <typename T>
template <class Self>
Self Math::SegmentedFunction<T>::operator+(this const Self &self,
    const Self &other)
{
	Self res;
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
			res.stops.emplace_back(it0->pos, it0->value + it1->value);
			++it1;
			++it0;
		}
	}
	return res;
}

#endif
