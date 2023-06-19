#ifndef BUBBLECHARTBUILDER_H
#define BUBBLECHARTBUILDER_H

#include <algorithm>
#include <cmath>

#include "bubblechart_impl.h"

namespace Vizzu
{
namespace Charts
{

typedef std::unordered_map<uint64_t, std::map<uint64_t, uint64_t>>
    Hierarchy;

class BubbleChartBuilder
{
public:
	template <typename Item>
	static void setupVector(std::vector<Item> &vector,
	    double maxRadius,
	    const Hierarchy &hierarchy);
};

template <typename Item>
void BubbleChartBuilder::setupVector(
    std::vector<Item> &items,
    double maxRadius,
    const Hierarchy &hierarchy)
{
	if (items.empty()) return;

	std::vector<double> sizes;
	for (auto &level : hierarchy) {
		auto sum = 0.0;
		for (auto &item : level.second)
			if (items[item.second].sizeFactor > 0)
				sum += items[item.second].sizeFactor;
		sizes.push_back(sum);
	}
	BubbleChartImpl chart(sizes);

	size_t cnt = 0;
	for (auto &level : hierarchy) {
		const auto &c = chart.getData()[cnt].circle;

		std::vector<double> sizes;
		for (auto &item : level.second)
			sizes.push_back(
			    std::max(0.0, items[item.second].sizeFactor));

		BubbleChartImpl subChart(sizes, c.boundary());

		size_t subCnt = 0;
		for (auto &item : level.second) {
			const auto &c = subChart.getData()[subCnt].circle;

			items[item.second].position =
			    Geom::Point(0.5 + (c.center.x - 0.5),
			        0.5 + (c.center.y - 0.5));

			auto r = c.radius;
			items[item.second].size = Geom::Size(r, r);
			items[item.second].sizeFactor =
			    r * r / (maxRadius * maxRadius);
			if (std::isnan(r)) items[item.second].enabled = false;
			subCnt++;
		}
		cnt++;
	}
}

}
}

#endif
