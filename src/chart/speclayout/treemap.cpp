#include "treemap.h"

#include <algorithm>

#include "base/math/interpolation.h"

namespace Vizzu::Charts
{

TreeMap::TreeMap(const std::vector<double> &sizes,
    const SpecMarker *parent)
{
	markers.reserve(sizes.size());

	for (auto j = 0U; j < sizes.size(); ++j)
		markers.emplace_back(j,
		    std::abs(sizes[j]),
		    std::signbit(sizes[j]));

	std::ranges::sort(markers, SpecMarker::sizeOrder);

	auto start = markers.begin();
	while (start != markers.end() && !std::isfinite(start->size()))
		start++->emplaceRect({0, 0}, {0, 0});

	divide(start,
	    markers.end(),
	    parent ? parent->rect().pos : Geom::Point{0, 1},
	    parent ? parent->rect().topRight() : Geom::Point{1, 0});

	std::ranges::sort(markers, SpecMarker::indexOrder);
}

void TreeMap::divide(It begin,
    It end,
    const Geom::Point &p0,
    const Geom::Point &p1,
    bool horizontal)
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