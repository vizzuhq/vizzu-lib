#ifndef TEXT_SMARTSTRING
#define TEXT_SMARTSTRING

#include <cctype>
#include <functional>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

#include "base/text/numberscale.h"

namespace Text
{

enum class NumberFormat { none, grouped, prefixed };

class SmartString
{
public:
	static std::vector<std::string> split(const std::string &str,
	    char delim,
	    bool ignoreEmpty = false,
	    const char *parens = nullptr);

	static void trim(std::string &string,
	    int (*ignore)(int) = &isspace);

	static std::string fromNumber(double value,
	    NumberFormat format = NumberFormat::none,
	    size_t maxFractionDigits = 2,
	    const NumberScale &numberScale = NumberScale{},
	    const std::string &unit = "");

	static std::string escape(const std::string &str);
};

}

#endif
