#include "naturalcmp.h"

#include <compare>
#include <string>

#include "character.h"

namespace Text
{

namespace SC = Character;

NaturalCmp::NaturalCmp(bool ignoreCase, bool ignoreSpace) :
    ignoreCase(ignoreCase),
    ignoreSpace(ignoreSpace)
{}

bool NaturalCmp::operator()(const std::string &op0,
    const std::string &op1) const
{
	return std::is_lt(cmp(op0.c_str(), op1.c_str()));
}

std::weak_ordering NaturalCmp::cmp(const char *s0,
    const char *s1) const
{
	auto res{std::weak_ordering::equivalent};
	while (std::is_eq(res) && (*s0 != '\0' || *s1 != '\0')) {
		if (ignoreSpace) {
			skipSpaces(s0);
			skipSpaces(s1);
		}

		if (SC::isDigit(*s0) && SC::isDigit(*s1))
			res = cmpNum(s0, s1);
		else
			res = cmpChar(*s0++, *s1++);
	}
	return res;
}

std::weak_ordering NaturalCmp::cmpChar(const char &s0,
    const char &s1) const
{
	return ignoreCase ? SC::toUpper(s0) <=> SC::toUpper(s1)
	                  : s0 <=> s1;
}

std::weak_ordering NaturalCmp::cmpNum(const char *&s0,
    const char *&s1)
{
	double v0{};
	double v1{};
	while (SC::isDigit(*s0) || SC::isDigit(*s1)) {
		if (SC::isDigit(*s0)) v0 = v0 * 10 + SC::toNumber(*s0++);
		if (SC::isDigit(*s1)) v1 = v1 * 10 + SC::toNumber(*s1++);
	}
	return std::strong_order(v0, v1);
}

void NaturalCmp::skipSpaces(const char *&s)
{
	while (SC::isSpace(*s)) ++s;
}

}