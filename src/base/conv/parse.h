#ifndef CONV_PARSE
#define CONV_PARSE

#include <optional>
#include <string>

#include "base/refl/auto_enum.h"
#include "base/type/traits.h"

namespace Conv
{

template <class T>
concept Parsable = !std::is_void_v<decltype(T::fromString(
    std::declval<std::string>()))>;

template <class To>
constexpr inline static bool IsParsable =
    Refl::is_enum<To> || Parsable<To>
    || (Type::is_optional_v<To> && IsParsable<Type::optional_t<To>>)
    || std::is_constructible_v<To, std::string>
    || std::is_same_v<To, bool> || std::is_floating_point_v<To>
    || std::is_integral_v<To>;

template <> constexpr inline bool IsParsable<void> = false;

template <typename To>
    requires IsParsable<To>
[[nodiscard]] decltype(auto) parse(const std::string &string)
{
	if constexpr (Refl::is_enum<To>)
		return Refl::get_enum<To>(string);
	else if constexpr (Parsable<To>)
		return To::fromString(string);
	else if constexpr (Type::is_optional_v<To>) {
		if (string == "null")
			return std::optional<std::remove_cvref_t<
			    decltype(parse<Type::optional_t<To>>(string))>>{};
		return std::optional{parse<Type::optional_t<To>>(string)};
	}
	else if constexpr (std::is_same_v<std::remove_const_t<To>,
	                       std::string>)
		return (string);
	else if constexpr (std::is_constructible_v<To, std::string>)
		return To(string);
	else if constexpr (std::is_same_v<To, bool>)
		return string == "true"
		    || (string != "false" && (throw std::bad_cast(), true));
	else if constexpr (std::is_floating_point_v<To>)
		return static_cast<To>(strtod(string.c_str(), nullptr));
	else {
		static_assert(std::is_integral_v<To>);
		return static_cast<To>(strtoll(string.c_str(), nullptr, 10));
	}
}

}

#endif
