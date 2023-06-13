#ifndef TABLECHART_H
#define TABLECHART_H

#include <cmath>

#include "chart/generator/plot.h"

namespace Vizzu
{
namespace Charts
{

class TableChart
{
public:
	template <typename Item>
	static void setupVector(std::vector<Item> &items,
	    bool singleColumn = false);
};

template <typename Item>
void TableChart::setupVector(std::vector<Item> &items,
    bool singleColumn)
{
	if (items.empty()) return;

	auto size = 0;
	for (auto &item : items)
		if (item.enabled) size++;

	size_t rowsize = (size_t)(singleColumn ? 1.0 : ceil(sqrt(size)));
	size_t colsize = (size_t)ceil((double)size / rowsize);
	size_t cnt = 0;

	for (auto &item : items) {
		item.spacing = Geom::Size(1, 1);
		if (item.enabled) {
			Geom::Point pos(1.0 + (double)(cnt % rowsize),
			    floor(cnt / rowsize));
			pos = pos / Geom::Point((double)rowsize, (double)colsize);
			item.position = Geom::Point(pos.x, 1 - pos.y);
			item.size = Geom::Point(1, 1)
			          / Geom::Point((double)rowsize, (double)colsize);
			cnt++;
		}
	}
}

}
}

#endif
