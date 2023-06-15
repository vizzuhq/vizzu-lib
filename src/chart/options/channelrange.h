#ifndef CHANNELRANGE_H
#define CHANNELRANGE_H

#include <optional>

#include "base/math/range.h"
#include "base/type/physicalvalue.h"

#include "autoparam.h"

namespace Vizzu
{
namespace Gen
{

enum class ChannelExtremaType {
	absolute, relative, minOffset, maxOffset
};

consteval auto unique_enum_names(ChannelExtremaType) {
	return ",%,min,max";
}

class ChannelExtrema :
    public Type::PhysicalValue<double, ChannelExtremaType>
{
public:
	typedef Type::PhysicalValue<double, ChannelExtremaType> Base;
	using Base::PhysicalValue;
	explicit ChannelExtrema(const std::string &str);
	explicit operator std::string() const;
};

typedef Base::AutoParam<ChannelExtrema> OptionalChannelExtrema;

class ChannelRange
{
public:
	OptionalChannelExtrema min;
	OptionalChannelExtrema max;

	Math::Range<double> getRange(
	    const Math::Range<double> &original) const;

	bool operator==(const ChannelRange &other) const
	{
		return min == other.min && max == other.max;
	}

private:
	double getExtrema(const OptionalChannelExtrema &extrema,
	    double original,
	    const Math::Range<double> &originalRange) const;
};

}
}

#endif
