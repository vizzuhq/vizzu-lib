#include "channelrange.h"

#include <string>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"
#include "base/math/range.h"
#include "base/text/valueunit.h"

namespace Vizzu::Gen
{

ChannelExtrema::ChannelExtrema(const std::string &str)
{
	const Text::ValueUnit vu(str);
	value = vu.getValue();
	unit = Conv::parse<ChannelExtremaType>(vu.getUnit());
}

ChannelExtrema::operator std::string() const
{
	return std::to_string(value) + std::string{Conv::toString(unit)};
}

Math::Range<double> ChannelRange::getRange(
    const Math::Range<double> &original) const
{
	return Math::Range<double>::Raw(
	    getExtrema(min, original.getMin(), original),
	    getExtrema(max, original.getMax(), original));
}

double ChannelRange::getExtrema(const OptionalChannelExtrema &extrema,
    double original,
    const Math::Range<double> &originalRange)
{
	if (!static_cast<bool>(extrema)) return original;

	typedef ChannelExtremaType ET;
	switch ((*extrema).unit) {
	case ET::absolute: return (*extrema).value;
	case ET::relative:
		return originalRange.scale((*extrema).value / 100.0);
	case ET::minOffset:
		return originalRange.getMin() + (*extrema).value;
	case ET::maxOffset:
	default: return originalRange.getMax() + (*extrema).value;
	}
}

}