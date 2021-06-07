#include "texttype.h"

#include <algorithm>
#include <iostream>
#include <locale>

#include "base/io/log.h"

using namespace Vizzu;
using namespace Vizzu::Data;

static const char decimalSeparator =
#ifndef VIZZU_NO_STDCOUT
	std::use_facet< std::numpunct<char> >(std::cout.getloc()).decimal_point();
#else
	'.';
#endif

TextType::TextType()
{
	count = 0;
	type = Number;
}

TextType::TextType(const Type &type, int count) :
	type(type),
	count(count)
{}

void TextType::update(const std::string &value)
{
	switch (type)
	{
	case Number:
		if(!value.empty() && !isNumber(value))
		{
			if (count > 5)
			{
				IO::Log("Warning: first " + std::to_string(count) + " cell was number, "
					"possible error in data file, "
					"row: " + std::to_string(count) + ", value: '" + value);
			}
			type = String;
		}
		break;

	default:
		break;
	}
	count++;
}

TextType::Type TextType::getType() const
{
	return type;
}

bool TextType::isNumber(const std::string &value)
{
	if (value.empty() || value.c_str() == nullptr) return false;
	try
	{
		size_t length = 0;
		std::stod(value, &length);
		if (length == value.size())
			return true;

		auto otherDecSep = decimalSeparator == '.' ? ',' : '.';
		auto converted = value;
		std::replace(converted.begin(), converted.end(),
					 otherDecSep, decimalSeparator);

		std::stod(converted, &length);
		if (length == converted.size())
			return true;

		return false;
	}
	catch(...)
	{
		return false;
	}
}
