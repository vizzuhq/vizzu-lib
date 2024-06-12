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
	auto size = 0;
	for (auto &item : items)
		if (item.enabled) ++size;

	if (size == 0) return;

	ssize_t rowsize{1};
	if (!singleColumn) {
		rowsize = static_cast<ssize_t>(ceil(sqrt(size)));
		auto sq = static_cast<ssize_t>(ceil(sqrt(
		    16.0 * static_cast<double>(rowsize * rowsize) / 9.0)));

		auto rem = rowsize - 1 - (size - 1) % rowsize;
		for (auto i = rowsize + 1; rem > 0 && i < sq; ++i)
			if (auto newRem = i - 1 - (size - 1) % i; newRem < rem) {
				rowsize = i;
				rem = newRem;
			}
	}
	auto colsize = ceil(
	    static_cast<double>(size) / static_cast<double>(rowsize));

	auto &&markerSize =
	    Geom::Point{1.0 / static_cast<double>(rowsize),
	        1.0 / colsize};

	for (ssize_t cnt{}; auto &item : items)
		if (item.enabled) {
			item.spacing = {1, 1};
			auto div = std::div(cnt++, rowsize);
			item.position = {static_cast<double>(div.rem + 1)
			                     / static_cast<double>(rowsize),
			    1.0 - static_cast<double>(div.quot) / colsize};
			item.size = markerSize;
		}
}

}

#endif
