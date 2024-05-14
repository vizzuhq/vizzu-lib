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
	for (auto it = begin; it != end; ++it) sum += it->size();

	if (sum == 0) {
		for (auto it = begin; it != end; ++it)
			it->emplaceRect(p0, p1);
		return;
	}

	double factor{};
	auto it = begin;
	while (it != end)
		if (factor += it++->size() / sum; factor > 0.4) break;

	auto &&[px, py] = Math::interpolate(p0, p1, factor);

	if (horizontal) {
		divide(begin, it, p0, Geom::Point{p1.x, py}, false);
		divide(it, end, Geom::Point{p0.x, py}, p1, false);
	}
	else {
		divide(begin, it, p0, Geom::Point{px, p1.y}, true);
		divide(it, end, Geom::Point{px, p0.y}, p1, true);
	}
}

}