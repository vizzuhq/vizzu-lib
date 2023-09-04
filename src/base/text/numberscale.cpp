#include "numberscale.h"

#include "base/conv/parse.h"
#include "base/text/smartstring.h"

using namespace Text;

NumberScale::NumberScale(PrefixType type)
{
	static const std::vector<std::vector<std::string>> prefixes{
	    {"k", "M", "G", "T", "P", "E", "Z", "Y"},
	    {"K", "M", "B", "T"},
	    {"k", "m", "bn", "tn"}};

	if (const auto u_type = static_cast<unsigned>(type);
	    u_type < prefixes.size()) {
		this->prefixes = prefixes.at(u_type);
	}
	else
		throw std::logic_error("invalid number scale type");
}

NumberScale::NumberScale(std::string s)
{
	Text::SmartString::trim(s);

	if (s.empty()) throw std::logic_error("empty number scale");

	auto items = SmartString::split(s, ',');

	if (items.size() == 1) {
		prefixes =
		    NumberScale(Conv::parse<PrefixType>(items[0])).prefixes;
	}
	else {
		for (auto &item : items) SmartString::trim(item);

		prefixes = items;
	}
}

std::string NumberScale::at(std::size_t index) const
{
	return index == 0 ? "" : prefixes.at(index - 1);
}

NumberScale::operator std::string() const
{
	std::string res;
	for (const auto &item : prefixes) {
		res += (!res.empty() ? "," : "");
		res += item;
	}
	return res;
}
