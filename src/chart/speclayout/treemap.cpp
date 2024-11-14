#include "treemap.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>

#include "base/geom/orientation.h"
#include "base/math/interpolation.h"

#include "specmarker.h"

namespace Vizzu::Charts
{

TreeMap::TreeMap(const std::vector<double> &sizes,
    const SpecMarker *parent)
{
	markers.reserve(sizes.size());

	for (auto j = 0U; const double &size : sizes)
		markers.emplace_back(j++, std::abs(size), std::signbit(size));

	std::ranges::stable_sort(markers, SpecMarker::sizeOrder);

	auto start = markers.begin();
	while (start != markers.end() && !std::isfinite(start->size()))
		start++->emplaceRect({0, 0}, {0, 0});

	divide(start,
	    markers.end(),
	    parent ? parent->rect().pos : Geom::Point{0, 1},
	    parent ? parent->rect().topRight() : Geom::Point{1, 0});

	std::ranges::stable_sort(markers, SpecMarker::indexOrder);
}

void TreeMap::divide(It begin,
    It end,
    const Geom::Point &p0,
    const Geom::Point &p1,
    Geom::Orientation orientation)
{
	while (begin != end && begin->negative)
		begin++->emplaceRect({0, 0}, {0, 0});
	while (begin != end && std::prev(end)->negative)
		(--end)->emplaceRect({0, 0}, {0, 0});

	if (begin == end) return;

	if (begin + 1 == end) {
		begin->emplaceRect(p0, p1);
		return;
	}

	auto sum = 0.0;
	for (auto it = begin; it != end; ++it)
		if (!it->negative) sum += it->size();

	if (sum == 0) {
		for (auto it = begin; it != end; ++it)
			it->emplaceRect(p0, p1);
		return;
	}

	double factor{};
	auto it = begin;
	for (; it != end; ++it)
		if (!it->negative)
			if (factor += it->size() / sum; factor > 0.4) {
				++it;
				break;
			}

	auto &&[px, py] = Math::interpolate(p0, p1, factor);

	using enum Geom::Orientation;
	if (isHorizontal(orientation)) {
		divide(begin, it, p0, Geom::Point{p1.x, py}, vertical);
		divide(it, end, Geom::Point{p0.x, py}, p1, vertical);
	}
	else {
		divide(begin, it, p0, Geom::Point{px, p1.y}, horizontal);
		divide(it, end, Geom::Point{px, p0.y}, p1, horizontal);
	}
}

}