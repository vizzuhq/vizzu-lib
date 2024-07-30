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

template <class T>
concept ToStringMember =
    requires(const T &t) { static_cast<std::string>(t.toString()); };

template <typename From>
    requires(ToStringMember<From> || std::is_enum_v<From>
             || Type::is_optional_v<From>
             || std::is_constructible_v<std::string, From>
             || std::is_arithmetic_v<From>)
std::string toString(const From &value)
{
	if constexpr (std::is_enum_v<From>) {
		return Refl::enum_name<std::string>(value);
	}
	else if constexpr (Type::is_optional_v<From>) {
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
	else if constexpr (ToStringMember<From>) {
		return value.toString();
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
