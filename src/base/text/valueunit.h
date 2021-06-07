#ifndef TEXT_VALUEUNIT
#define TEXT_VALUEUNIT

#include <string>

namespace Text
{

class ValueUnit
{
public:
	explicit ValueUnit(const std::string &str);
	double getValue() const { return value; }
	const std::string &getUnit() const { return unit; }

private:
	double value;
	std::string unit;
};

}

#endif
