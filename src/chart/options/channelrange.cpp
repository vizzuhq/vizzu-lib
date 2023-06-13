#include "channelrange.h"

#include "base/conv/tostring.h"
#include "base/text/valueunit.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

ChannelExtrema::ChannelExtrema(const std::string &str)
{
	Text::ValueUnit vu(str);
	value = vu.getValue();
	unit = ChannelExtremaType(vu.getUnit());
}

ChannelExtrema::operator std::string() const
{
	return std::to_string(value) + (std::string)unit;
}

Math::Range<double> ChannelRange::getRange(
    const Math::Range<double> &original) const
{
	return Math::Range<double>(
	    getExtrema(min, original.getMin(), original),
	    getExtrema(max, original.getMax(), original));
}

double ChannelRange::getExtrema(const OptionalChannelExtrema &extrema,
    double original,
    const Math::Range<double> &originalRange) const
{
	if (!(bool)extrema) return original;

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
