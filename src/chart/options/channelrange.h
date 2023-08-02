#ifndef CHANNELRANGE_H
#define CHANNELRANGE_H

#include <optional>

#include "base/math/range.h"
#include "base/type/physicalvalue.h"

#include "autoparam.h"

namespace Vizzu::Gen
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
	using Base = Type::PhysicalValue<double, ChannelExtremaType>;
	using Base::PhysicalValue;
	explicit ChannelExtrema(const std::string &str);
	explicit operator std::string() const;
};

using OptionalChannelExtrema = Base::AutoParam<ChannelExtrema>;

class ChannelRange
{
public:
	OptionalChannelExtrema min;
	OptionalChannelExtrema max;

	[[nodiscard]] Math::Range<double> getRange(
	    const Math::Range<double> &original) const;

	bool operator==(const ChannelRange &other) const
	{
		return min == other.min && max == other.max;
	}

private:
	static double getExtrema(const OptionalChannelExtrema &extrema,
	    double original,
	    const Math::Range<double> &originalRange) ;
};

}

#endif
