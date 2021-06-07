#include "naturalcmp.h"

#include <cctype>

#include "character.h"

using namespace Text;

using SC = Character;

NaturalCmp::NaturalCmp(bool ignoreCase, bool ignoreSpace)
	: ignoreCase(ignoreCase),
	  ignoreSpace(ignoreSpace)
{}

bool NaturalCmp::operator()(const std::string &op0, const std::string &op1) const
{
	const auto *s0 = op0.c_str();
	const auto *s1 = op1.c_str();
	auto res = cmp(s0, s1);
	return res == -1;
}

int NaturalCmp::cmp(const char *&s0, const char *&s1) const
{
	while (true)
	{
		if (ignoreSpace)
		{
			skipSpaces(s0);
			skipSpaces(s1);
		}

		if (SC::isDigit(*s0) && SC::isDigit(*s1))
		{
			auto res = cmpNum(s0, s1);
			if (res != 0) return res;
		}

		auto c0 = *s0;
		auto c1 = *s1;
		if (!c0 && !c1) return 0;

		auto res = cmpChar(s0, s1);
		if (res != 0) return res;

		++s0; ++s1;
	}
}

int NaturalCmp::cmpChar(const char *&s0, const char *&s1) const
{
	auto c0 = *s0;
	auto c1 = *s1;

	if (ignoreCase) {
		c0 = SC::toUpper(c0);
		c1 = SC::toUpper(c1);
	}

	if (c0 < c1) return -1;
	if (c0 > c1) return 1;
	return 0;
}

int NaturalCmp::cmpNum(const char *&s0, const char *&s1)
{
	double v0 = 0;
	double v1 = 0;
	while(true)
	{
		if (!SC::isDigit(*s0) && !SC::isDigit(*s1))
			return v0 < v1 ? -1 : v0 > v1 ? 1 : 0;

		if (SC::isDigit(*s0)) v0 = v0 * 10 + SC::toNumber(*s0);
		if (SC::isDigit(*s1)) v1 = v1 * 10 + SC::toNumber(*s1);

		if (SC::isDigit(*s0)) s0++;
		if (SC::isDigit(*s1)) s1++;
	}
}

void NaturalCmp::skipSpaces(const char *&s)
{
	while (SC::isSpace(*s)) s++;
}

