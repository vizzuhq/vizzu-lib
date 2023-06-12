#ifndef SCALERANGE_H
#define SCALERANGE_H

#include <optional>

#include "base/math/range.h"
#include "base/refl/enum.h"
#include "base/type/physicalvalue.h"

#include "autoparam.h"

namespace Vizzu
{
namespace Diag
{

class SpecNameEnum(ScaleExtremaType)(absolute,
    relative,
    minOffset,
    maxOffset)(, %, min, max);

class ScaleExtrema :
    public Type::PhysicalValue<double, ScaleExtremaType>
{
public:
	typedef Type::PhysicalValue<double, ScaleExtremaType> Base;
	using Base::PhysicalValue;
	explicit ScaleExtrema(const std::string &str);
	explicit operator std::string() const;
};

typedef Base::AutoParam<ScaleExtrema> OptionalScaleExtrema;

class ScaleRange
{
public:
	OptionalScaleExtrema min;
	OptionalScaleExtrema max;

	Math::Range<double> getRange(
	    const Math::Range<double> &original) const;

	bool operator==(const ScaleRange &other) const
	{
		return min == other.min && max == other.max;
	}

private:
	double getExtrema(const OptionalScaleExtrema &extrema,
	    double original,
	    const Math::Range<double> &originalRange) const;
};

}
}

#endif
