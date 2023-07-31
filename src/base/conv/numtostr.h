#ifndef CONV_NUMTOSTR
#define CONV_NUMTOSTR

#include <array>
#include <cmath>
#include <string>
#include <type_traits>

namespace Conv
{

class NumberToString
{
public:
	int fractionDigitCount;
	bool fillFractionWithZero;
	char integerGgrouping;
	char fractionGgrouping;
	char decimalPointChar;

	NumberToString();
	[[nodiscard]] std::string convert(double number);
	[[nodiscard]] std::string operator()(double number);

private:
	constexpr static inline auto MAX_BUFFER_SIZE = 430;
	std::array<char, MAX_BUFFER_SIZE> buffer;
};

}

#endif
