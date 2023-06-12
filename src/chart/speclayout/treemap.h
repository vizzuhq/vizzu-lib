#ifndef TREEMAP_H
#define TREEMAP_H

#include <cstddef>
#include <map>
#include <unordered_map>
#include <vector>

#include "base/geom/rect.h"

namespace Vizzu
{
namespace Charts
{

typedef std::unordered_map<uint64_t, std::map<uint64_t, uint64_t>>
    Hierarchy;

class TreeMap
{
public:
	TreeMap(const std::vector<double> &sizes,
	    const Geom::Point &p0 = Geom::Point(0, 1),
	    const Geom::Point &p1 = Geom::Point(1, 0));

	template <typename Item>
	static void setupVector(std::vector<Item> &vector,
	    const Hierarchy &hierarchy);

private:
	struct SizeRecord
	{
		size_t index;
		double value;
	};

	struct DataRecord
	{
		size_t index;
		Geom::Point p0;
		Geom::Point p1;
	};

	typedef std::vector<SizeRecord>::const_iterator It;

	std::vector<SizeRecord> sums;
	std::vector<DataRecord> data;

	void divide(It begin,
	    It end,
	    const Geom::Point &p0,
	    const Geom::Point &p1,
	    bool horizontal = true);
};

template <typename Item>
void TreeMap::setupVector(std::vector<Item> &items,
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
	TreeMap chart(sizes);

	size_t cnt = 0;
	for (auto &level : hierarchy) {
		auto &c = chart.data[cnt];

		std::vector<double> sizes;
		for (auto &item : level.second)
			sizes.push_back(items[item.second].sizeFactor);

		TreeMap subChart(sizes, c.p0, c.p1);

		size_t subCnt = 0;
		for (auto &item : level.second) {
			auto &c = subChart.data[subCnt];
			Geom::Rect rect(c.p0, c.p1 - c.p0);
			rect = rect.positive();
			items[item.second].position = rect.topRight();
			items[item.second].size = rect.size;
			subCnt++;
		}

		cnt++;
	}
}

}
}

#endif
