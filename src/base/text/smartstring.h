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

	static std::string join(
	    const auto &vector,
	    char separator = ',')
	{
		std::string joined;
		bool needSep = false;
		for (const auto &s : vector) {
			if (needSep)
				joined += separator;
			else
				needSep = true;
			joined.append(s);
		}
		return joined;
	}

	static void trim(std::string &string,
	    int (*ignore)(int) = &isspace);

	static std::string fromNumber(double value,
	    NumberFormat format = NumberFormat::none,
	    size_t maxFractionDigits = 2,
	    const NumberScale &numberScale = NumberScale::siSymbols);

	static std::string escape(const std::string &str,
	    char specChar = '\"');
};

}

#endif
