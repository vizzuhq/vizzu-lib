
#include "smartstring.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <locale>
#include <sstream>

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
	string.erase(std::find_if_not(string.rbegin(), string.rend(),
				 [=](int c){return c >= 0 && c <= 255 && ignore(c); }).base(),
	             string.end());
}

void SmartString::leftTrim(std::string &string, int (*ignore)(int))
{
	string.erase(string.begin(),
	             std::find_if_not(string.begin(), string.end(),
								  [=](int c){return  c >= 0 && c <= 255 && ignore(c); }));
}

void SmartString::trim(std::string &string, int (*ignore)(int))
{
	leftTrim(string, ignore);
	rightTrim(string, ignore);
}

void SmartString::trimBOM(std::string &string)
{
	if (string.size() >= 3) {
		if (string[0] == (char)0xEF
			&& string[1] == (char)0xBB
			&& string[2] == (char)0xBF)
		{
			string = string.substr(3, std::string::npos);
		}
	}
}

std::string SmartString::fromNumber(double value, NumberFormat format)
{
	switch(format) {
	case NumberFormat::prefixed: {
		return humanReadable(value);
	}
	case NumberFormat::grouped: {
		struct my_numpunct : std::numpunct<char> {
			char do_thousands_sep() const override { return ' '; }
			std::string do_grouping() const override {return "\03";}
		};
		std::stringstream ss;
		std::locale loc(ss.getloc(),new my_numpunct);
		ss.imbue(loc);
		ss.precision(10);
		ss << value;
		return ss.str();
	}
	default:
	case NumberFormat::none: {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
	};
}

std::vector<std::string> SmartString::split(const std::string &str,
    char delim,
    bool ignoreEmpty)
{
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (!ignoreEmpty || !item.empty()) result.push_back(item);
	}
	return result;
}

std::vector<std::string> SmartString::split(const std::string &str,
    const std::string &delim,
    bool ignoreEmpty)
{
	std::vector<std::string> result;

	if (delim.empty())
	{
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

		if (subend == str.end())
			break;

		substart = subend + delim.size();
	}
	return result;
}

std::string SmartString::humanReadable(double value, int digits,
	const std::vector<std::string> &prefixes)
{
	Math::EngineeringNumber num(value);

	std::string res = std::to_string(num.coefficient);

	if (!Math::Floating(num.coefficient).isInteger()
		&& num.coefficient < pow(10, digits - 1))
	{
		res = res.substr(0, digits + 1);
	}
	else
	{
		res = res.substr(0, res.find_first_of('.'));
	}

	if (res.find('.') != std::string::npos)
		while (res.size() > 1 && (res.back() == '0' || res.back() == '.')) {
			auto ch = res.back();
			res.resize(res.size() - 1);
			if (ch == '.') break;
		}

	if(!num.positive)
		res = '-' + res;

	if (num.exponent >= 0 && num.exponent < (int)prefixes.size())
	{
		auto prefix = prefixes.at(num.exponent);
		return res + (!prefix.empty() ? " " + prefix : "");
	}
	else return fromNumber(value);
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

std::string SmartString::escape(const std::string &str, const char *charList)
{
	std::string result;
	for (const auto &ch : str)
	{
		bool needsEscape = (ch == '\\') || strchr(charList, ch);
		if (needsEscape) result += "\\";
		result += ch;
	}
	return result;
}

}
