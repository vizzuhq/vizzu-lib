#ifndef CONV_NUMTOSTR
#define CONV_NUMTOSTR

#include <array>
#include <string>

namespace Conv
{

struct NumberToString
{
	int fractionDigitCount = 6;
	bool fillFractionWithZero = false;
	char integerGgrouping = '\0';
	char fractionGgrouping = '\0';
	char decimalPointChar = '.';
	constexpr static auto MAX_BUFFER_SIZE = 430;
	std::array<char, MAX_BUFFER_SIZE> buffer = {};

	[[nodiscard]] std::string convert(double number);
	[[nodiscard]] std::string operator()(double number);
};

}

#endif
