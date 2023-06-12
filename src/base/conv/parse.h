#ifndef CONV_PARSE
#define CONV_PARSE

#include <string>
#include <optional>

#include "base/type/traits.h"
#include "base/conv/strtonum.h"

namespace Conv
{

template <typename To>
To parse(const std::string &string)
{
	if constexpr (Type::isoptional<To>::value)
	{
		if (string == "null") return std::nullopt;
		else return parse<typename To::value_type>(string);
	}
	else if constexpr (std::is_constructible_v<To, std::string>)
	{
		return To(string);
	}
	else if constexpr (std::is_same_v<To,bool>)
	{
		return string == "true" ? true :
		       string == "false" ? false :
		       throw std::bad_cast();
	}
	else if constexpr (std::is_floating_point<To>::value)
	{
		return (To)strtod(string.c_str(), nullptr);
	}
	else if constexpr (std::is_integral<To>::value)
	{
		return (To)strtoll(string.c_str(), nullptr, 10);
	}
	else []<bool flag = false>()
	{
		static_assert(flag, "no conversion from string");
	}();
}

}

#endif
