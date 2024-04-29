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
	    static_cast<ssize_t>(singleColumn ? 1.0 : ceil(sqrt(size)));
	auto colsize = static_cast<size_t>(ceil(
	    static_cast<double>(size) / static_cast<double>(rowsize)));
	ssize_t cnt = 0;

	for (auto &item : items) {
		item.spacing = Geom::Size{1, 1};
		if (item.enabled) {
			auto div = std::div(cnt, rowsize);
			Geom::Point pos(1.0 + static_cast<double>(div.rem),
			    static_cast<double>(div.quot));
			pos = pos
			    / Geom::Point{static_cast<double>(rowsize),
			        static_cast<double>(colsize)};
			item.position = Geom::Point{pos.x, 1 - pos.y};
			item.size = Geom::Point{1, 1}
			          / Geom::Point{static_cast<double>(rowsize),
			              static_cast<double>(colsize)};
			++cnt;
		}
	}
}

}

#endif
