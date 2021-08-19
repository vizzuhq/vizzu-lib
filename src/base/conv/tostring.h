#ifndef CONV_TOSTRING
#define CONV_TOSTRING

#include <sstream>
#include <string>
#include <optional>

#include "base/type/traits.h"

namespace Conv
{

template <typename From>
std::string toString(const From &value)
{
	if constexpr (Type::isoptional<From>::value)
	{
		if (!value) return "null";
		else return toString(*value);
	}
	else if constexpr (std::is_constructible_v<std::string, From>)
	{
		return std::string(value);
	}
	else if constexpr (std::is_same_v<From,bool>)
	{
		return value ? "true" : "false";
	}
	else if constexpr (Type::ostreamable<From>)
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
	else []<bool flag = false>()
	{
		static_assert(flag, "no string conversion");
	}();
}

}

#endif
