#include "numberscale.h"

#include "base/text/smartstring.h"
#include "base/conv/parse.h"

using namespace Text;

const NumberScale NumberScale::siSymbols(PrefixType::SISymbol);

NumberScale::NumberScale(PrefixType type)
{
	static const std::vector<std::vector<std::string>> prefixes{
		{"k", "M", "G", "T", "P", "E", "Z", "Y"},
		{"K", "M", "B", "T"},
		{"k", "m", "bn", "tn"}
	};

	if ((unsigned)type < prefixes.size())
	{
		this->prefixes = prefixes.at((unsigned)type);
	}
	else throw std::logic_error("invalid number scale type");
}

NumberScale::NumberScale(std::string s)
{
	Text::SmartString::trim(s);

	if (s.empty()) throw std::logic_error("empty number scale");

	auto items = SmartString::split(s, ',');

	if (items.size() == 1)
	{
		*this = NumberScale(Conv::parse<PrefixType>(items[0]));
	}
	else
	{
		for (auto &item: items)
			SmartString::trim(item);

		prefixes = items;
	}
}

std::string NumberScale::at(std::size_t index) const { 
	if (index == 0) return "";
	else return prefixes.at(index-1); 
}

NumberScale::operator std::string() const
{
	std::string res;
	for (const auto &item: prefixes)
	{
		res += (!res.empty() ? "," : "");
		res += item;
	}
	return res;
}

