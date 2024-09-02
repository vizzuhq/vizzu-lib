#ifndef MATH_SEGMENTEDFUNC_TPP
#define MATH_SEGMENTEDFUNC_TPP

#include <ranges>

#include "base/math/interpolation.h"
#include "base/math/range.h"

#include "segmentedfunc.h"

template <typename T, class CRTP>
T Math::SegmentedFunction<T, CRTP>::operator()(double pos) const
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

#endif
