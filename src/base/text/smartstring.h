#ifndef TEXT_SMARTSTRING
#define TEXT_SMARTSTRING

#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "numberscale.h"

namespace Text
{

enum class NumberFormat : std::uint8_t { none, grouped, prefixed };

namespace SmartString
{
template <char... separators>
constexpr std::string join(std::span<const std::string_view> il)
{
	std::string res;
	for (auto sl : il) {
		if (!res.empty()) {
			static_cast<void>(((res += separators, 0), ...));
		}
		res += sl;
	}
	return res;
}

[[nodiscard]] std::vector<std::string> split(const std::string &str,
    char delim,
    bool ignoreEmpty = false,
    const char *parens = nullptr);

std::string_view trim_view(const std::string_view &string);

void trim(std::string &string);

[[nodiscard]] std::string fromPhysicalValue(double value,
    NumberFormat format,
    size_t maxFractionDigits,
    const NumberScale &numberScale,
    const std::string &unit);
}

}

#endif
