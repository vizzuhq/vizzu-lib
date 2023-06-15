#ifndef TEXT_SMARTSTRING
#define TEXT_SMARTSTRING

#include <cctype>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "base/text/numberscale.h"

namespace Text
{

enum class NumberFormat { none, grouped, prefixed };

class SmartString
{
public:
	static void lowerCase(std::string &str);
	static void upperCase(std::string &str);
	static std::string lowerCase(const std::string &str);
	static std::string upperCase(const std::string &str);

	static bool startsWith(const std::string &str,
	    const std::string &substr)
	{
		return (str.rfind(substr, 0) == 0);
	}

	static std::vector<std::string> split(const std::string &str,
	    char delim,
	    bool ignoreEmpty = false,
	    const char *parens = nullptr);
	static std::vector<std::string> split(const std::string &str,
	    const std::string &delim,
	    bool ignoreEmpty = false);

	template <template <class, class> class container = std::vector,
	    typename T>
	static std::string join(
	    const container<T, std::allocator<T>> &vector,
	    std::string separator)
	{
		std::string joined;
		for (auto &s : vector)
			joined +=
			    (!joined.empty() ? separator : "") + std::string(s);
		return joined;
	}

	static std::vector<std::string> map(const auto &container,
	    const auto &transform)
	{
		std::vector<std::string> res;
		for (auto &val : container) res.push_back(transform(val));
		return res;
	}

	/** Removes trailing whitespaces from the string */
	static void rightTrim(std::string &string,
	    int (*ignore)(int) = &isspace);

	/** Removes whitespaces from the begining of the string */
	static void leftTrim(std::string &string,
	    int (*ignore)(int) = &isspace);

	/** Removes whitespaces from both ends of the string */
	static void trim(std::string &string,
	    int (*ignore)(int) = &isspace);

	static void trimBOM(std::string &string);

	static std::string fromNumber(double value, size_t digits);

	static std::string fromNumber(double value,
	    NumberFormat format = NumberFormat::none,
	    size_t maxFractionDigits = 2,
	    const NumberScale &numberScale = NumberScale::siSymbols);

	static std::string humanReadable(double value,
	    int maxFractionDigits = 2,
	    const NumberScale &numberScale = NumberScale::siSymbols);

	static std::string escape(const std::string &str,
	    const char *charList);

	static std::string deescape(const std::string &str);
};

}

#endif
