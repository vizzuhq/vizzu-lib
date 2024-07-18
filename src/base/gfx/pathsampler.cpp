#include "pathsampler.h"

#include "base/geom/triangle.h"

namespace Gfx
{

void PathSampler::calc()
{
	auto point0 = getPoint(0.0);
	auto point1 = getPoint(1.0);

	addPoint(point0);
	path(point0, point1, 0.0, 1.0);
	addPoint(point1);
}

void PathSampler::path(const Geom::Point &pConv0,
    const Geom::Point &pConv1,
    double i0,
    double i1,
    size_t recurseCnt)
{
	const size_t maxRecursion = 20;
	if (recurseCnt >= maxRecursion) return;

	auto i = (i0 + i1) / 2.0;
	auto pConv = getPoint(i);

	const Geom::Triangle triangle{{pConv0, pConv, pConv1}};
	auto area = triangle.area();
	auto height = 2 * area / (pConv1 - pConv0).abs();

	auto needMore =
	    height > hMax
	    || ((pConv1 - pConv0).sqrAbs() < (pConv - pConv0).sqrAbs())
	    || ((pConv1 - pConv0).sqrAbs() < (pConv - pConv1).sqrAbs());

	if (needMore) {
		if ((pConv - pConv0).sqrAbs() > dMax)
			path(pConv0, pConv, i0, i, recurseCnt + 1);
	}

	addPoint(pConv);

	if (needMore) {
		if ((pConv - pConv1).sqrAbs() > dMax)
			path(pConv, pConv1, i, i1, recurseCnt + 1);
	}
}

}