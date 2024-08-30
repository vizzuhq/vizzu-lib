
#include "smartstring.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "base/conv/numtostr.h"
#include "base/math/normalizednumber.h"

#include "numberscale.h"

namespace Text::SmartString
{
void trim(std::string &string)
{
	auto ignore = [](unsigned char c)
	{
		return std::isspace(c);
	};
	string.erase(string.begin(),
	    std::find_if_not(string.begin(), string.end(), ignore));
	string.erase(
	    std::find_if_not(string.rbegin(), string.rend(), ignore)
	        .base(),
	    string.end());
}

std::vector<std::string> split(const std::string &str,
    char delim,
    bool ignoreEmpty,
    const char *parens)
{
	int nestingLevel = 0;
	std::string tmp;
	std::vector<std::string> result;
	for (auto c : str) {
		if (parens) {
			if (c == parens[0]) ++nestingLevel;
			if (c == parens[1]) --nestingLevel;
		}
		if (c == delim && nestingLevel <= 0) {
			if (!tmp.empty() || !ignoreEmpty)
				result.emplace_back(std::move(tmp));
			tmp.clear();
		}
		else
			tmp += c;
	}
	if (!str.empty() && (!tmp.empty() || !ignoreEmpty))
		result.emplace_back(std::move(tmp));
	return result;
}

std::string fromPhysicalValue(double value,
    NumberFormat format,
    size_t maxFractionDigits,
    const NumberScale &numberScale,
    const immutable_string &unit)
{
	Conv::NumberToString converter{
	    static_cast<int>(maxFractionDigits)};
	switch (format) {
	case NumberFormat::prefixed: {
		Math::EngineeringNumber num(value);

		if (num.exponent >= 0) {
			if (num.exponent >= static_cast<int>(numberScale.size()))
				num.setExponent(
				    static_cast<int>(numberScale.size() - 1));

			auto prefix = numberScale.at(num.exponent);

			return converter(num.signedCoef())
			     + (prefix.empty() && (unit.empty() || unit == "%")
			             ? unit.toString()
			             : " " + prefix + unit);
		}
		break;
	}
	case NumberFormat::grouped:
		converter.integerGgrouping = ' ';
		[[fallthrough]];
	case NumberFormat::none:
	default: break;
	}
	return converter(value)
	     + (unit.empty() || unit == "%" ? unit.toString()
	                                    : " " + unit);
}

}
