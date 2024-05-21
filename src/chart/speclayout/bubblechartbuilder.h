#ifndef BUBBLECHARTBUILDER_H
#define BUBBLECHARTBUILDER_H

#include <algorithm>
#include <cmath>

#include "bubblechart.h"
#include "sizedependentlayout.h"

namespace Vizzu::Charts
{

class BubbleChartBuilder : SizeDependentLayout<BubbleChart>
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
	SizeDependentLayout::setupVector(hierarchy,
	    [&maxRadius](auto *item, const SpecMarker &marker)
	    {
		    if (const auto &[center, r] = marker.circle();
		        std::isnan(r))
			    item->enabled = false;
		    else {
			    item->position = center;
			    item->size = Geom::Size{r, r};
			    item->sizeFactor = r * r / (maxRadius * maxRadius);
		    }
	    });
}

}

#endif
