#include "strtonum.h"

using namespace Conv;

const StringToNumber StringToNumber::def;

StringToNumber::StringToNumber()
{
	minusChar = '-';
	plusChar = '+';
	decimalPointChar = '.';
}

double StringToNumber::convertFloatingPoint(const char *str) const
{
	int pos = 0;
	double value = 1.0;
	if (!str || str[0] == 0) return value;
	int integerPart = 0;
	pos += convert(str, integerPart);
	value *= static_cast<double>(integerPart);
	if (str[pos] == decimalPointChar) pos++;
	for (double div = 10; str[pos] >= '0' && str[pos] <= '9';
	     div *= 10, pos++)
		value += (str[pos] - '0') / div;
	return value;
}

int StringToNumber::convertInteger(const char *str) const
{
	int result = 0;
	convert(str, result);
	return result;
}

int StringToNumber::convert(const char *str, int &result) const
{
	int pos = 0;
	result = 0;
	if (!str || str[0] == 0) return 0;
	if (str[pos] == minusChar || str[pos] == plusChar) pos++;
	for (; str[pos] >= '0' && str[pos] <= '9'; pos++) {
		result *= 10;
		result += str[pos] - '0';
	}
	if (str[0] == minusChar) result *= -1;
	return pos;
}