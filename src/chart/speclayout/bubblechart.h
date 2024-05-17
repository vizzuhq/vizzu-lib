#ifndef BUBBLECHART_H
#define BUBBLECHART_H

#include <vector>

#include "base/geom/circle.h"
#include "base/geom/point.h"

#include "specmarker.h"

namespace Vizzu::Charts
{

class BubbleChart
{
public:
	using Markers = std::vector<SpecMarker>;

	Markers markers;

	explicit BubbleChart(const std::vector<double> &circleAreas,
	    const Geom::Rect &rect = Geom::Rect(Geom::Point{0, 0},
	        Geom::Size{1, 1}));

private:
	void generate();

	void normalize(const Geom::Rect &rect);

	[[nodiscard]] std::optional<Geom::Circle> getTouchingCircle(
	    const SpecMarker &act,
	    size_t firstIdx,
	    size_t lastIdx) const;
};

}

#endif
