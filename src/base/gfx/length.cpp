#include "length.h"

#include <stdexcept>

using namespace Gfx;

Length::Length(const std::string &s)
{
	const Text::ValueUnit parser(s);
	if (const auto &unit = parser.getUnit(); unit == "%") {
		relative = parser.getValue() / 100.0;
		absolute = 0.0;
		emphemeral = 0.0;
	}
	else if (unit == "em") {
		absolute = 0.0;
		relative = 0.0;
		emphemeral = parser.getValue();
	}
	else if (unit == "px" || unit.empty()) {
		absolute = parser.getValue();
		relative = 0.0;
		emphemeral = 0.0;
	}
	else
		throw std::logic_error("invalid length unit: " + unit);
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
