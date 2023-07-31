#ifndef BASE_TEXT_VALUEUNIT_H
#define BASE_TEXT_VALUEUNIT_H

#include <string>

namespace Text
{

class ValueUnit
{
public:
	explicit ValueUnit(const std::string &str);
	[[nodiscard]] double getValue() const { return value; }
	[[nodiscard]] const std::string &getUnit() const { return unit; }

private:
	double value;
	std::string unit;
};

}

#endif
