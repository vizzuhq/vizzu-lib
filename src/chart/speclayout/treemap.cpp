#include "treemap.h"

#include <algorithm>

#include "base/math/interpolation.h"

Vizzu::Charts::TreeMap::TreeMap(const std::vector<double> &sizes,
    const Geom::Point &p0,
    const Geom::Point &p1)
{
	markers.reserve(sizes.size());

	for (auto j = 0U; j < sizes.size(); j++)
		markers.emplace_back(j, sizes[j]);

	std::sort(markers.begin(), markers.end(), SpecMarker::sizeOrder);

	divide(markers.begin(), markers.end(), p0, p1);

	std::sort(markers.begin(), markers.end(), SpecMarker::indexOrder);
}

void Vizzu::Charts::TreeMap::divide(It begin,
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
		divide(begin, it, p0, Geom::Point{p1.x, p.y}, !horizontal);
	else
		divide(begin, it, p0, Geom::Point{p.x, p1.y}, !horizontal);

	if (horizontal)
		divide(it, end, Geom::Point{p0.x, p.y}, p1, !horizontal);
	else
		divide(it, end, Geom::Point{p.x, p0.y}, p1, !horizontal);
}
