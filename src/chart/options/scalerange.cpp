#include "scalerange.h"

#include "base/text/valueunit.h"
#include "base/conv/tostring.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

ScaleExtrema::ScaleExtrema(const std::string &str)
{
	Text::ValueUnit vu(str);
	value = vu.getValue();
	unit = ScaleExtremaType(vu.getUnit());
}

ScaleExtrema::operator std::string() const
{
	return std::to_string(value) + (std::string)unit;
}

Math::Range<double> 
ScaleRange::getRange(const Math::Range<double> &original) const
{
	return Math::Range<double>(
		getExtrema(min, original.getMin(), original),
		getExtrema(max, original.getMax(), original)
	);
}

double ScaleRange::getExtrema(
	const OptionalScaleExtrema &extrema, 
	double original,
	const Math::Range<double> &originalRange) const
{
	if (!(bool)extrema) return original;

	typedef ScaleExtremaType ET;
	switch(extrema->unit)
	{
		case ET::absolute: return extrema->value;
		case ET::relative: return originalRange.scale(extrema->value / 100.0);
		case ET::minOffset: return originalRange.getMin() + extrema->value;
		case ET::maxOffset: return originalRange.getMax() + extrema->value;
	}
}
