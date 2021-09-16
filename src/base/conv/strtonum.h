#ifndef CONV_STRTONUM
#define CONV_STRTONUM

#include <string>
#include <type_traits>
#include <cmath>

namespace Conv
{

class StringToNumber {
public:
    static const StringToNumber def;

    char minusChar;
    char plusChar;
    char decimalPointChar;

    StringToNumber();
    int convertInteger(const char* str) const;
    double convertFloatingPoint(const char* str) const;

private:
    int convert(const char* str, int& result) const;
};

}

#endif //CONV_STRTONUM
