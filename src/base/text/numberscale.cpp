#include "numberscale.h"

#include <cstddef>
#include <stdexcept>
#include <string>

#include "base/conv/parse.h"
#include "base/text/smartstring.h"

namespace Text
{

NumberScale::NumberScale(PrefixType type)
{
	switch (type) {
	case PrefixType::SISymbol:
		prefixes.assign({"k", "M", "G", "T", "P", "E", "Z", "Y"});
		break;
	case PrefixType::shortScaleSymbolUS:
		prefixes.assign({"K", "M", "B", "T"});
		break;
	case PrefixType::shortScaleSymbolUK:
		prefixes.assign({"k", "m", "bn", "tn"});
		break;
	default: throw std::logic_error("invalid number scale type");
	}
}

NumberScale::NumberScale(std::string s)
{
	SmartString::trim(s);

	if (s.empty()) throw std::logic_error("empty number scale");

	prefixes = SmartString::split(s, ',');

	if (prefixes.size() == 1)
		prefixes = NumberScale(Conv::parse<PrefixType>(prefixes[0]))
		               .prefixes;
	else
		for (auto &item : prefixes) SmartString::trim(item);
}

std::string NumberScale::at(std::size_t index) const
{
	return index == 0 ? "" : prefixes.at(index - 1);
}

NumberScale::operator std::string() const
{
	return Text::SmartString::join<','>(prefixes);
}

}