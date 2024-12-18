#include "channelrange.h"

#include "base/math/range.h"

namespace Vizzu::Gen
{

Math::Range<> ChannelRange::getRange(
    const Math::Range<> &original) const
{
	return {getExtrema(min, original.min, original),
	    getExtrema(max, original.max, original)};
}

double ChannelRange::getExtrema(const OptionalChannelExtrema &extrema,
    double original,
    const Math::Range<> &originalRange)
{
	if (!static_cast<bool>(extrema)) return original;

	typedef ChannelExtremaType ET;
	switch ((*extrema).unit) {
	case ET::absolute: return (*extrema).value;
	case ET::relative:
		return originalRange.scale((*extrema).value / 100.0);
	case ET::minOffset: return originalRange.min + (*extrema).value;
	case ET::maxOffset:
	default: return originalRange.max + (*extrema).value;
	}
}

}