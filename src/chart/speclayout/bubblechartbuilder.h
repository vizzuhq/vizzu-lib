#ifndef BUBBLECHARTBUILDER_H
#define BUBBLECHARTBUILDER_H

#include <algorithm>
#include <cmath>

#include "bubblechart.h"

namespace Vizzu::Charts
{

class BubbleChartBuilder
{
public:
	template <typename Hierarchy>
	static void setupVector(double maxRadius,
	    const Hierarchy &hierarchy);
};

template <typename Hierarchy>
void BubbleChartBuilder::setupVector(double maxRadius,
    const Hierarchy &hierarchy)
{
	if (hierarchy.empty()) return;

	std::vector<double> sizes(hierarchy.size());
	for (std::size_t ix{}; const auto &level : hierarchy)
		for (auto &sum = sizes[ix++]; const auto &item : level)
			if (item->sizeFactor > 0) sum += item->sizeFactor;

	const BubbleChart chart(sizes);

	std::vector<double> ssizes(hierarchy.inner_size());
	for (std::size_t cnt{}; const auto &level : hierarchy) {
		for (std::size_t ix{}; const auto &item : level)
			ssizes[ix++] = item->sizeFactor;

		const BubbleChart subChart(ssizes,
		    chart.markers[cnt++].circle().boundary());

		for (std::size_t subCnt{}; const auto &item : level)
			if (const auto &[center, r] =
			        subChart.markers[subCnt++].circle();
			    std::isnan(r))
				item->enabled = false;
			else {
				item->position = center;
				item->size = Geom::Size{r, r};
				item->sizeFactor = r * r / (maxRadius * maxRadius);
			}
	}
}

}

#endif
