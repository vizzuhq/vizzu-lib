#ifndef TREEMAP_H
#define TREEMAP_H

#include <cstddef>
#include <map>
#include <vector>

#include "base/geom/rect.h"

#include "specmarker.h"

namespace Vizzu::Charts
{

class TreeMap
{
public:
	explicit TreeMap(const std::vector<double> &sizes,
	    const Geom::Point &p0 = Geom::Point{0, 1},
	    const Geom::Point &p1 = Geom::Point{1, 0});

	template <typename Hierarchy>
	static void setupVector(const Hierarchy &hierarchy);

private:
	using It = std::vector<SpecMarker>::iterator;

	std::vector<SpecMarker> markers;

	void divide(It begin,
	    It end,
	    const Geom::Point &p0,
	    const Geom::Point &p1,
	    bool horizontal = true);
};

template <typename Hierarchy>
void TreeMap::setupVector(const Hierarchy &hierarchy)
{
	if (hierarchy.empty()) return;

	std::vector<double> sizes(hierarchy.size());
	for (std::size_t ix{}; const auto &level : hierarchy)
		for (auto &sum = sizes[ix++]; const auto &item : level)
			if (item->sizeFactor > 0) sum += item->sizeFactor;

	TreeMap chart(sizes);

	for (std::size_t cnt{}; const auto &level : hierarchy) {
		auto &&[pos, size] = chart.markers[cnt++].rect();

		std::vector<double> sizes(level.size());
		for (std::size_t ix{}; const auto &item : level)
			sizes[ix++] = item->sizeFactor;

		TreeMap subChart(sizes, pos, pos + size);

		for (size_t subCnt{}; const auto &item : level) {
			auto &&[spos, ssize] =
			    subChart.markers[subCnt++].rect().positive();
			item->position = spos + ssize;
			item->size = ssize;
		}
	}
}

}

#endif
