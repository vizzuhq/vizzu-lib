#ifndef CONV_NUMTOSTR
#define CONV_NUMTOSTR

#include <string>
#include <type_traits>
#include <cmath>

namespace Conv
{

class NumberToString {
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
    std::string buffer;

    void integerToString(uint64_t num);
    void fractionToString(double num);
};

}

#endif
