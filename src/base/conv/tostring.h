#ifndef CONV_TOSTRING
#define CONV_TOSTRING

#include <optional>
#include <string>
#include <type_traits>

#include "base/refl/auto_enum.h"
#include "base/type/traits.h"

#include "numtostr.h"

namespace Conv
{

template <typename From>
auto toString(const From &value) -> std::enable_if_t<
    std::is_enum_v<From> || Type::isoptional<From>::value
        || std::is_constructible_v<std::string, From>
        || std::is_arithmetic_v<From>,
    std::string>
{
	if constexpr (std::is_enum_v<From>) {
		return Refl::enum_name(value);
	}
	else if constexpr (Type::isoptional<From>::value) {
		if (!value) return "null";
		return toString(*value);
	}
	else if constexpr (std::is_constructible_v<std::string, From>) {
		return static_cast<std::string>(value);
	}
	else if constexpr (std::is_same_v<From, bool>) {
		return value ? "true" : "false";
	}
	else if constexpr (std::is_arithmetic_v<From>) {
		return std::to_string(value);
	}
	else
		[]<bool flag = false>()
		{
			static_assert(flag, "no string conversion");
		}
	();
}

}

#endif
