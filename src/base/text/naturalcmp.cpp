#include "naturalcmp.h"

#include <cctype>

#include "character.h"

using CharTraits = Text::Character;

Text::NaturalCmp::NaturalCmp(bool ignoreCase, bool ignoreSpace) :
    ignoreCase(ignoreCase),
    ignoreSpace(ignoreSpace)
{}

bool Text::NaturalCmp::operator()(const std::string &op0,
    const std::string &op1) const
{
	const auto *s0 = op0.c_str();
	const auto *s1 = op1.c_str();
	auto res = cmp(s0, s1);
	return res == -1;
}

int Text::NaturalCmp::cmp(const char *&s0, const char *&s1) const
{
	int res = cmpChar(s0, s1);
	for (; res != 0 && *s0 != '\0'; ++s0, ++s1) {
		if (ignoreSpace) {
			skipSpaces(s0);
			skipSpaces(s1);
		}

		if (CharTraits::isDigit(*s0) && CharTraits::isDigit(*s1)) {
			res = cmpNum(s0, s1);
			if (res != 0) break;
		}
		res = cmpChar(s0, s1);
	}
	return res;
}

int Text::NaturalCmp::cmpChar(const char *&s0, const char *&s1) const
{
	auto c0 = *s0;
	auto c1 = *s1;

	if (ignoreCase) {
		c0 = CharTraits::toUpper(c0);
		c1 = CharTraits::toUpper(c1);
	}

	if (c0 < c1) return -1;
	if (c0 > c1) return 1;
	return 0;
}

int Text::NaturalCmp::cmpNum(const char *&s0, const char *&s1)
{
	double v0 = 0;
	double v1 = 0;
	while (CharTraits::isDigit(*s0) || CharTraits::isDigit(*s1)) {
		if (CharTraits::isDigit(*s0))
			v0 = v0 * 10 + CharTraits::toNumber(*s0);
		if (CharTraits::isDigit(*s1))
			v1 = v1 * 10 + CharTraits::toNumber(*s1);

		if (CharTraits::isDigit(*s0)) s0++;
		if (CharTraits::isDigit(*s1)) s1++;
	}
	return v0 < v1 ? -1 : v0 > v1 ? 1 : 0;
}

void Text::NaturalCmp::skipSpaces(const char *&s)
{
	while (CharTraits::isSpace(*s)) s++;
}
