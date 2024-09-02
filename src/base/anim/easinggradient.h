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
	friend struct SegmentedFunction;
	using SegmentedFunction::SegmentedFunction;

	[[nodiscard]] static EasingGradient Bezier(const Geom::Point &p1,
	    const Geom::Point &p2,
	    size_t stepCount = 10);

	using SegmentedFunction::operator();
};

}

#endif
