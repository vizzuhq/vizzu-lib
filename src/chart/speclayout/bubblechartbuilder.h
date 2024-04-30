#ifndef BUBBLECHARTBUILDER_H
#define BUBBLECHARTBUILDER_H

#include <algorithm>
#include <cmath>

#include "bubblechart.h"

namespace Vizzu::Charts
{

template <class Item>
using Hierarchy = std::vector<std::vector<Item>>;

class BubbleChartBuilder
{
public:
	template <typename Item>
	static void setupVector(double maxRadius,
	    const Hierarchy<Item> &hierarchy);
};

template <typename Item>
void BubbleChartBuilder::setupVector(double maxRadius,
    const Hierarchy<Item> &hierarchy)
{
	if (hierarchy.empty()) return;

	std::vector<double> sizes(hierarchy.size());
	for (std::size_t ix{}; const auto &level : hierarchy)
		for (auto &sum = sizes[ix++]; const auto &item : level)
			if (item->sizeFactor > 0) sum += item->sizeFactor;

	const BubbleChart chart(sizes);

	for (std::size_t cnt{}; const auto &level : hierarchy) {
		std::vector<double> ssizes(level.size());
		for (std::size_t ix{}; const auto &item : level)
			if (auto &s = ssizes[ix++]; item->sizeFactor > 0)
				s = item->sizeFactor;

		const BubbleChart subChart(ssizes,
		    chart.markers[cnt++].circle().boundary());

		for (std::size_t subCnt{}; const auto &item : level) {
			const auto &[center, r] =
			    subChart.markers[subCnt++].circle();
			item->position = center;
			item->size = Geom::Size{r, r};
			item->sizeFactor = r * r / (maxRadius * maxRadius);
			if (std::isnan(r)) item->enabled = false;
		}
	}
}

}

#endif
