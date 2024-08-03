#include "valueunit.h"

#include <cstddef>
#include <string>

#include "base/text/smartstring.h"

namespace Text
{

ValueUnit::ValueUnit(const std::string &str)
{
	size_t next{};
	value = std::stod(str, &next);
	unit = str.substr(next);
	SmartString::trim(unit);
}

}