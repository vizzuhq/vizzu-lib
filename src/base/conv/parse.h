#ifndef CONV_PARSE
#define CONV_PARSE

#include <optional>
#include <string>

#include "base/refl/auto_enum.h"
#include "base/type/traits.h"

namespace Conv
{

template <typename To> To parse(const std::string &string)
{
	if constexpr (std::is_enum_v<To>) {
		return Refl::get_enum<To>(string);
	}
	else if constexpr (Type::isoptional<To>::value) {
		if (string == "null")
			return std::nullopt;
		return parse<typename To::value_type>(string);
	}
	else if constexpr (std::is_constructible_v<To, std::string>) {
		return To(string);
	}
	else if constexpr (std::is_same_v<To, bool>) {
		auto s = string == "true";
		return s || string == "false" ? s : throw std::bad_cast();
	}
	else if constexpr (std::is_floating_point<To>::value) {
		return static_cast<To>(strtod(string.c_str(), nullptr));
	}
	else if constexpr (std::is_integral<To>::value) {
		return static_cast<To>(strtoll(string.c_str(), nullptr, 10));
	}
	else
		[]<bool flag = false>()
		{
			static_assert(flag, "no conversion from string");
		}
	();
}

}

#endif
