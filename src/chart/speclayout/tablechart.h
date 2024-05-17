#ifndef TABLECHART_H
#define TABLECHART_H

#include <cmath>

namespace Vizzu::Charts
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
		if (item.enabled) ++size;

	auto rowsize =
	    singleColumn ? 1 : static_cast<ssize_t>(ceil(sqrt(size)));
	auto colsize = ceil(
	    static_cast<double>(size) / static_cast<double>(rowsize));

	auto &&markerSize =
	    Geom::Point{1.0 / static_cast<double>(rowsize),
	        1.0 / colsize};

	for (ssize_t cnt{}; auto &item : items)
		if (item.enabled) {
			item.spacing = {1, 1};
			auto div = std::div(cnt++, rowsize);
			item.position = {(1.0 + static_cast<double>(div.rem))
			                     / static_cast<double>(rowsize),
			    1.0 - static_cast<double>(div.quot) / colsize};
			item.size = markerSize;
		}
}

}

#endif
