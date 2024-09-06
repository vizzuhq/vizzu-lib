#ifndef TEXT_SMARTSTRING
#define TEXT_SMARTSTRING

#include <cstdint>
#include <string>
#include <vector>

#include "base/text/numberscale.h"

namespace Text
{

enum class NumberFormat : std::uint8_t { none, grouped, prefixed };

namespace SmartString
{
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
