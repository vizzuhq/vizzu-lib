#ifndef BUBBLECHARTBUILDER_H
#define BUBBLECHARTBUILDER_H

#include <algorithm>
#include <cmath>

#include "bubblechart.h"

namespace Vizzu::Charts
{

using Hierarchy = std::vector<std::vector<uint64_t>>;

class BubbleChartBuilder
{
public:
	template <typename Item>
	static void setupVector(std::vector<Item> &items,
	    double maxRadius,
	    const Hierarchy &hierarchy);
};

template <typename Item>
void BubbleChartBuilder::setupVector(std::vector<Item> &items,
    double maxRadius,
    const Hierarchy &hierarchy)
{
	if (items.empty()) return;

	std::vector<double> sizes;
	for (const auto &level : hierarchy) {
		auto sum = 0.0;
		for (const auto &item : level)
			if (items[item].sizeFactor > 0)
				sum += items[item].sizeFactor;
		sizes.push_back(sum);
	}

	const BubbleChart chart(sizes);

	size_t cnt = 0;
	for (const auto &level : hierarchy) {
		const auto &c = chart.markers[cnt].circle();

		std::vector<double> sizes;
		sizes.reserve(std::size(level));
		for (const auto &item : level)
			sizes.push_back(std::max(0.0, items[item].sizeFactor));

		const BubbleChart subChart(sizes, c.boundary());

		size_t subCnt = 0;
		for (const auto &item : level) {
			const auto &c = subChart.markers[subCnt].circle();

			items[item].position =
			    Geom::Point{0.5 + (c.center.x - 0.5),
			        0.5 + (c.center.y - 0.5)};

			auto r = c.radius;
			items[item].size = Geom::Size{r, r};
			items[item].sizeFactor = r * r / (maxRadius * maxRadius);
			if (std::isnan(r)) items[item].enabled = false;
			++subCnt;
		}
		++cnt;
	}
}

}

#endif
