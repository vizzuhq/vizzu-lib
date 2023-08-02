#ifndef MATH_SEGMENTEDFUNC_TPP
#define MATH_SEGMENTEDFUNC_TPP

#include "segmentedfunc.h"

#include "base/math/interpolation.h"
#include "base/math/range.h"

using namespace Math;

template <typename T> T
SegmentedFunction<T>::at(double pos) const
{
	if (stops.empty()) return T();
	if (stops.size() == 1) return stops.at(0).value;

	if (pos < stops.front().pos) return stops.front().value;
	if (pos > stops.back().pos) return stops.back().value;

	for (auto i = 1U; i < stops.size(); i++)
	{
		if (pos >= stops.at(i - 1).pos && pos <= stops.at(i).pos)
		{
			const Range range(stops.at(i - 1).pos, stops.at(i).pos);

			auto factor = range.rescale(pos);

			return interpolate(stops.at(i - 1).value,
				stops.at(i).value,
				factor);
		}
	}

	return stops.back().value;
}

template <typename T>
bool SegmentedFunction<T>::operator==(const SegmentedFunction &other) const
{
	return stops == other.stops;
}

template <typename T>
SegmentedFunction<T> SegmentedFunction<T>::operator*(double value) const
{
	auto res = *this;
	for (auto &stop : res.stops) stop.value = stop.value * value;
	return res;
}

template <typename T>
SegmentedFunction<T>
SegmentedFunction<T>::operator+(const SegmentedFunction &other) const
{
	SegmentedFunction<T> res;

	auto it0 = stops.begin();
	auto it1 = other.stops.begin();
	while (it0 != stops.end() || it1 != other.stops.end())
	{
		if (it1 == other.stops.end() || it0->pos < it1->pos)
		{
			res.stops.emplace_back(it0->pos, it0->value + other.at(it0->pos));
			++it0;
		}
		else if (it0 == stops.end() || it1->pos < it0->pos)
		{
			res.stops.emplace_back(it1->pos, it1->value + at(it1->pos));
			++it1;
		}
		else
		{
			res.stops.emplace_back(it0->pos, it0->value + it1->value);
			++it1;
			++it0;
		}
	}
	return res;
}

#endif
