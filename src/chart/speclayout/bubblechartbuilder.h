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

	std::vector<double> sizes;
	for (const auto &level : hierarchy) {
		auto sum = 0.0;
		for (const auto &item : level)
			if (item->sizeFactor > 0) sum += item->sizeFactor;
		sizes.push_back(sum);
	}

	const BubbleChart chart(sizes);

	size_t cnt = 0;
	for (const auto &level : hierarchy) {
		const auto &c = chart.markers[cnt].circle();

		std::vector<double> sizes;
		sizes.reserve(std::size(level));
		for (const auto &item : level)
			sizes.push_back(std::max(0.0, item->sizeFactor));

		const BubbleChart subChart(sizes, c.boundary());

		size_t subCnt = 0;
		for (const auto &item : level) {
			const auto &c = subChart.markers[subCnt].circle();

			item->position = Geom::Point{0.5 + (c.center.x - 0.5),
			    0.5 + (c.center.y - 0.5)};

			auto r = c.radius;
			item->size = Geom::Size{r, r};
			item->sizeFactor = r * r / (maxRadius * maxRadius);
			if (std::isnan(r)) item->enabled = false;
			++subCnt;
		}
		++cnt;
	}
}

}

#endif
