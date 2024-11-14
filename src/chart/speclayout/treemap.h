#ifndef TREEMAP_H
#define TREEMAP_H

#include <cstddef>
#include <map>
#include <vector>

#include "base/geom/rect.h"

#include "sizedependentlayout.h"
#include "specmarker.h"

namespace Vizzu::Charts
{

class TreeMap
{
public:
	explicit TreeMap(const std::vector<double> &sizes,
	    const SpecMarker *parent = nullptr);

	std::vector<SpecMarker> markers;

private:
	using It = std::vector<SpecMarker>::iterator;

	static void divide(It begin,
	    It end,
	    const Geom::Point &p0,
	    const Geom::Point &p1,
	    Geom::Orientation orientation =
	        Geom::Orientation::horizontal);
};

struct TreeMapBuilder : SizeDependentLayout<TreeMap>
{
	template <typename Hierarchy>
	static void setupVector(const Hierarchy &hierarchy);
};

template <typename Hierarchy>
void TreeMapBuilder::setupVector(const Hierarchy &hierarchy)
{
	SizeDependentLayout::setupVector(hierarchy,
	    [](auto &item, const SpecMarker &marker)
	    {
		    auto &&[spos, ssize] = marker.rect().positive();
		    item.position = spos + ssize;
		    item.size = ssize;
	    });
}

}

#endif
