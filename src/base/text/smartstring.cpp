
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

void SmartString::lowerCase(std::string &str)
{
	for (auto &c : str) c = ::tolower(c);
}

void SmartString::upperCase(std::string &str)
{
	for (auto &c : str) c = ::toupper(c);
}

std::string SmartString::lowerCase(const std::string &str)
{
	std::string res(str);
	lowerCase(res);
	return res;
}

std::string SmartString::upperCase(const std::string &str)
{
	std::string res(str);
	upperCase(res);
	return res;
}

void SmartString::rightTrim(std::string &string, int (*ignore)(int))
{
	string.erase(std::find_if_not(string.rbegin(),
	                 string.rend(),
	                 [=](int c)
	                 {
		                 return c >= 0 && c <= 255 && ignore(c);
	                 })
	                 .base(),
	    string.end());
}

void SmartString::leftTrim(std::string &string, int (*ignore)(int))
{
	string.erase(string.begin(),
	    std::find_if_not(string.begin(),
	        string.end(),
	        [=](int c)
	        {
		        return c >= 0 && c <= 255 && ignore(c);
	        }));
}

void SmartString::trim(std::string &string, int (*ignore)(int))
{
	leftTrim(string, ignore);
	rightTrim(string, ignore);
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
			if (!tmp.empty() || !ignoreEmpty) result.push_back(tmp);
			tmp.clear();
		}
		else
			tmp += c;
	}
	if (!str.empty() && (!tmp.empty() || !ignoreEmpty))
		result.push_back(tmp);
	return result;
}

std::vector<std::string> SmartString::split(const std::string &str,
    const std::string &delim,
    bool ignoreEmpty)
{
	std::vector<std::string> result;

	if (delim.empty()) {
		result.push_back(str);
		return result;
	}

	auto substart = str.begin();
	while (true) {
		auto subend = std::search(substart,
		    str.end(),
		    delim.begin(),
		    delim.end());
		std::string temp(substart, subend);
		if (!ignoreEmpty || !temp.empty()) result.push_back(temp);

		if (subend == str.end()) break;

		substart = subend + delim.size();
	}
	return result;
}

std::string SmartString::fromNumber(double value, size_t digits)
{
	auto negative = value < 0;
	auto absValue = std::abs(value);

	std::string res = std::to_string(absValue);

	if (!Math::Floating(absValue).isInteger()
	    && absValue < pow(10, digits - 1)) {
		res = res.substr(0, digits + 1);
	}
	else {
		res = res.substr(0, res.find_first_of('.'));
	}

	if (res.find('.') != std::string::npos)
		while (res.size() > 1
		       && (res.back() == '0' || res.back() == '.')) {
			auto ch = res.back();
			res.resize(res.size() - 1);
			if (ch == '.') break;
		}

	if (negative) res = '-' + res;

	return res;
}

std::string SmartString::fromNumber(double value,
    NumberFormat format,
    size_t maxFractionDigits,
    const NumberScale &numberScale)
{
	switch (format) {
	case NumberFormat::prefixed: {
		return humanReadable(value, maxFractionDigits, numberScale);
	}
	case NumberFormat::grouped: {
		Conv::NumberToString converter;
		converter.fractionDigitCount = maxFractionDigits;
		converter.integerGgrouping = ' ';
		return converter(value);
	}
	default:
	case NumberFormat::none: {
		Conv::NumberToString converter;
		converter.fractionDigitCount = maxFractionDigits;
		return converter(value);
	}
	}
}

std::string SmartString::humanReadable(double value,
    int maxFractionDigits,
    const NumberScale &numberScale)
{
	Math::EngineeringNumber num(value);

	if (num.exponent >= 0) {
		if (num.exponent >= static_cast<int>(numberScale.size()))
			num.setExponent(numberScale.size() - 1);

		std::string res = fromNumber(num.signedCoef(),
		    NumberFormat::none,
		    maxFractionDigits);

		auto prefix = numberScale.at(num.exponent);

		return res + (!prefix.empty() ? " " + prefix : "");
	}
	else
		return fromNumber(value,
		    NumberFormat::none,
		    maxFractionDigits);
}

std::string SmartString::deescape(const std::string &str)
{
	std::string result;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '\\') i++;
		result += str[i];
	}
	return result;
}

std::string SmartString::escape(const std::string &str,
    const char *charList)
{
	std::string result;
	for (const auto &ch : str) {
		bool needsEscape = (ch == '\\') || strchr(charList, ch);
		if (needsEscape) result += "\\";
		result += ch;
	}
	return result;
}

}
