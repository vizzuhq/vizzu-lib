#include "pathsampler.h"

#include <cmath>
#include <cstddef>

#include "base/geom/point.h"
#include "base/geom/triangle.h"
#include "base/math/floating.h"

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
	constexpr static size_t maxRecursion = 20;
	if (recurseCnt >= maxRecursion) return;

	const auto i = (i0 + i1) / 2.0;
	const auto pConv = getPoint(i);

	const auto area = Geom::Triangle{{pConv0, pConv, pConv1}}.area();
	const auto sqrLength = (pConv1 - pConv0).sqrAbs();
	const auto height = Math::Floating::is_zero(sqrLength)
	                      ? 0.0
	                      : 2 * area / sqrt(sqrLength);

	const auto sqrAbs0 = (pConv - pConv0).sqrAbs();
	const auto sqrAbs1 = (pConv - pConv1).sqrAbs();

	auto needMore = height > options.curveHeightMax
	             || sqrLength < sqrAbs0 || sqrLength < sqrAbs1;

	if (needMore && sqrAbs0 > options.distanceMax)
		path(pConv0, pConv, i0, i, recurseCnt + 1);

	addPoint(pConv);

	if (needMore && sqrAbs1 > options.distanceMax)
		path(pConv, pConv1, i, i1, recurseCnt + 1);
}

}