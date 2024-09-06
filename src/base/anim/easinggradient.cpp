
#include "easinggradient.h"

#include <cstddef>
#include <stdexcept>

#include "base/geom/bezier.h"
#include "base/geom/point.h"

namespace Anim
{

EasingGradient EasingGradient::Bezier(const Geom::Point &p1,
    const Geom::Point &p2,
    std::size_t stepCount)
{
	if (stepCount < 2)
		throw std::logic_error("not enough bezier steps");

	if (p1.x < 0 || p1.x > 1 || p2.x < 0 || p2.x > 1)
		throw std::logic_error("bezier point is out of range");

	EasingGradient res(stepCount);

	const Geom::CubicBezier<Geom::Point> bezier(Geom::Point{0, 0},
	    p1,
	    p2,
	    Geom::Point{1, 1});

	for (auto i = 0U; i < stepCount; ++i) {
		auto point = bezier(static_cast<double>(i)
		                    / static_cast<double>(stepCount - 1));
		res.stops[i] = {point.x, point.y};
	}

	return res;
}

}
