#ifndef CONV_NUMTOSTR
#define CONV_NUMTOSTR

#include <string>
#include <type_traits>
#include <cmath>

namespace Conv
{

class NumberToString {
public:
    static const NumberToString def;

    int integerDigitCount;
    int fractionDigitCount;
    bool fillIntegerWithZero;
    bool fillFractionWithZero;
    char integerGgrouping;
    char fractionGgrouping;
    char signChar;
    char decimalPointChar;

    NumberToString();
    std::string convert(double number) const;
    std::string operator()(double number) const;

private:
    mutable std::string buffer;

    void integerToString(uint64_t num) const;
    void fractionToString(double num) const;
};

}

#endif //CONV_NUMTOSTR
