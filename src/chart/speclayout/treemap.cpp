#include "treemap.h"

#include <algorithm>

#include "base/math/interpolation.h"

namespace Vizzu::Charts
{

TreeMap::TreeMap(const std::vector<double> &sizes,
    const Geom::Point &p0,
    const Geom::Point &p1)
{
	markers.reserve(sizes.size());

	for (auto j = 0U; j < sizes.size(); ++j)
		markers.emplace_back(j, sizes[j]);

	std::ranges::sort(markers, SpecMarker::sizeOrder);

	auto it = std::ranges::upper_bound(markers,
	    SpecMarker{0, 0.0},
	    SpecMarker::sizeOrder);

	divide(markers.begin(), it, p0, p1);

	while (it != markers.end()) it++->emplaceRect({0, 0}, {0, 0});

	std::ranges::sort(markers, SpecMarker::indexOrder);
}

void TreeMap::divide(It begin,
    It end,
    const Geom::Point &p0,
    const Geom::Point &p1,
    bool horizontal)
{
	if (begin + 1 == end) {
		begin->emplaceRect(p0, p1);
		return;
	}

	auto sum = 0.0;
	for (auto it = begin; it != end; ++it) sum += it->size;

	if (sum == 0) {
		for (auto it = begin; it != end; ++it)
			it->emplaceRect(p0, p1);
		return;
	}

	auto factor = 0.0;
	auto it = begin;
	for (; it != end; ++it) {
		if (sum > 0) factor += it->size / sum;
		if (factor > 0.4) {
			++it;
			break;
		}
	}

	auto p = Math::interpolate(p0, p1, factor);

	if (horizontal)
		divide(begin, it, p0, Geom::Point{p1.x, p.y}, false);
	else
		divide(begin, it, p0, Geom::Point{p.x, p1.y}, true);

	if (horizontal)
		divide(it, end, Geom::Point{p0.x, p.y}, p1, false);
	else
		divide(it, end, Geom::Point{p.x, p0.y}, p1, true);
}

}