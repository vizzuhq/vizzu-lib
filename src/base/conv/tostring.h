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

template <typename From> std::string toString(const From &value)
{
	if constexpr (std::is_enum_v<From>) {
		return Refl::enum_name(value);
	}
	else if constexpr (Type::isoptional<From>::value) {
		if (!value)
			return "null";
		return toString(*value);
	}
	else if constexpr (std::is_constructible<std::string,
	                       From>::value) {
		return std::string(value);
	}
	else if constexpr (std::is_same<From, bool>::value) {
		return value ? "true" : "false";
	}
	else if constexpr (std::is_arithmetic<From>::value) {
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
