#ifndef ANIM_EASINGGRADIENT
#define ANIM_EASINGGRADIENT

#include "base/geom/point.h"
#include "base/math/segmentedfunc.h"

#include "easing.h"

namespace Anim
{

class EasingGradient : protected Math::SegmentedFunction<double>
{
public:
	using Math::SegmentedFunction<double>::SegmentedFunction;

	static EasingGradient Bezier(const Geom::Point &p1,
	    const Geom::Point &p2,
	    size_t stepCount = 10);

	double operator()(double t) const { return at(t); }
};

}

#endif
