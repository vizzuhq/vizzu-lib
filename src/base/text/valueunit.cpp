#include "valueunit.h"

#include "base/text/smartstring.h"

using namespace Text;

ValueUnit::ValueUnit(const std::string &str)
{
	size_t next{};
	value = std::stod(str, &next);
	unit = str.substr(next);
	Text::SmartString::trim(unit);
}
