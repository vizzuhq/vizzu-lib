#include "length.h"

#include <stdexcept>

using namespace Gfx;

Length::Length(const std::string &s)
{
	Text::ValueUnit parser(s);
	auto unit = parser.getUnit();
	if (unit == "%")
	{
		relative = parser.getValue();
	}
	else if (unit == "px" || unit.empty())
	{
		absolute = parser.getValue();
	}
	else throw std::logic_error("invalid length unit: " + unit);
}

Length::operator std::string() const
{
	if (relative == 0.0) return std::to_string(absolute) + "px";
	if (absolute == 0.0) return std::to_string(relative) + "%";
	return std::to_string(absolute) + "px"
		+ "+" + std::to_string(relative) + "%";
}
