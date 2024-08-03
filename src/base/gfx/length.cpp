#include "length.h"

#include <stdexcept>
#include <string>

#include "base/text/valueunit.h"

namespace Gfx
{

Length Length::fromString(const std::string &s)
{
	Length res{};
	const Text::ValueUnit parser(s);
	if (const auto &unit = parser.getUnit(); unit == "%") {
		res.relative = parser.getValue() / 100.0;
	}
	else if (unit == "em") {
		res.emphemeral = parser.getValue();
	}
	else if (unit == "px" || unit.empty()) {
		res.absolute = parser.getValue();
	}
	else
		throw std::logic_error("invalid length unit: " + unit);

	return res;
}

Length::operator std::string() const
{
	if (isAbsolute()) return std::to_string(absolute) + "px";
	if (isRelative()) return std::to_string(100.0 * relative) + "%";
	if (isEmphemeral()) return std::to_string(emphemeral) + "em";
	return std::to_string(absolute) + "px" + "+"
	     + std::to_string(100.0 * relative) + "%" + "+"
	     + std::to_string(emphemeral) + "em";
}

}