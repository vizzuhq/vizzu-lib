#ifndef MATH_SEGMENTEDFUNC_TPP
#define MATH_SEGMENTEDFUNC_TPP

#include "base/math/interpolation.h"
#include "base/math/range.h"

#include "segmentedfunc.h"

template <typename T>
T Math::SegmentedFunction<T>::at(double pos) const
{
	if (stops.empty()) return T();
	if (stops.size() == 1) return stops.at(0).value;

	if (pos < stops.front().pos) return stops.front().value;
	if (pos > stops.back().pos) return stops.back().value;

	for (auto i = 1U; i < stops.size(); ++i) {
		auto &[ppos, pval] = stops[i - 1];
		auto &[cpos, cval] = stops[i];
		if (ppos <= pos && pos <= cpos)
			return interpolate(pval,
			    cval,
			    Range{ppos, cpos}.rescale(pos));
	}

	return stops.back().value;
}

template <typename T>
bool Math::SegmentedFunction<T>::operator==(
    const SegmentedFunction &other) const
{
	return stops == other.stops;
}

template <typename T>
Math::SegmentedFunction<T> Math::SegmentedFunction<T>::operator*(
    double value) const
{
	auto res = *this;
	for (auto &stop : res.stops) stop.value = stop.value * value;
	return res;
}

template <typename T>
Math::SegmentedFunction<T> Math::SegmentedFunction<T>::operator+(
    const SegmentedFunction &other) const
{
	SegmentedFunction<T> res;

	for (auto it0 = stops.begin(), it1 = other.stops.begin();
	     it0 != stops.end() || it1 != other.stops.end();) {
		if (it1 == other.stops.end() || it0->pos < it1->pos) {
			res.stops.emplace_back(it0->pos,
			    it0->value + other.at(it0->pos));
			++it0;
		}
		else if (it0 == stops.end() || it1->pos < it0->pos) {
			res.stops.emplace_back(it1->pos,
			    it1->value + at(it1->pos));
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
