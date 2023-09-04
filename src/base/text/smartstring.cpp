
#include "smartstring.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>

#include "base/conv/numtostr.h"
#include "base/math/floating.h"
#include "base/math/normalizednumber.h"

namespace Text
{
void SmartString::trim(std::string &string, int (*ignore)(int))
{
	string.erase(string.begin(),
	    std::find_if_not(string.begin(), string.end(), ignore));
	string.erase(
	    std::find_if_not(string.rbegin(), string.rend(), ignore)
	        .base(),
	    string.end());
}

std::vector<std::string> SmartString::split(const std::string &str,
    char delim,
    bool ignoreEmpty,
    const char *parens)
{
	int nestingLevel = 0;
	std::string tmp;
	std::vector<std::string> result;
	for (auto c : str) {
		if (parens) {
			if (c == parens[0]) nestingLevel++;
			if (c == parens[1]) nestingLevel--;
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

std::string SmartString::fromNumber(double value,
    NumberFormat format,
    size_t maxFractionDigits,
    const NumberScale &numberScale)
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
			     + (!prefix.empty() ? " " + prefix : "");
		}
		break;
	}
	case NumberFormat::grouped:
		converter.integerGgrouping = ' ';
		[[fallthrough]];
	case NumberFormat::none:
	default: break;
	}
	return converter(value);
}

std::string SmartString::escape(const std::string &str, char specChar)
{
	std::string result;
	for (const auto &ch : str) {
		if ((ch == '\\') || specChar == ch) result.push_back('\\');
		result.push_back(ch);
	}
	return result;
}

}
