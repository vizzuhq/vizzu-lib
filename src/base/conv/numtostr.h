#ifndef CONV_NUMTOSTR
#define CONV_NUMTOSTR

#include <string>
#include <type_traits>
#include <cmath>

namespace Conv
{

class NumberToString {
public:
    int integerDigitCount;
    int fractionDigitCount;
    bool fillIntegerWithZero;
    bool fillFractionWithZero;
    char signChar;
    char integerGgroupping;
    char fractionGgroupping;
    char decimalPointChar;

    NumberToString();
    std::string convert(double number);
    std::string operator()(double number);

private:
    std::string buffer;
    void integerToString(uint64_t num);
    void fractionToString(double num);
};

}

#endif //CONV_NUMTOSTR
