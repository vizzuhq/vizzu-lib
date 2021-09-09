#ifndef CONV_TOSTRING
#define CONV_TOSTRING

#include <sstream>
#include <string>
#include <optional>
#include <type_traits>

#include "base/type/traits.h"
#include "numtostr.h"

namespace Conv
{

template <typename From>
std::string toString(const From &value, const NumberToString& conv = NumberToString::def)
{
	if constexpr (Type::isoptional<From>::value)
	{
		if (!value) return "null";
		else return toString(*value);
	}
	else if constexpr (std::is_constructible<std::string, From>::value)
	{
		return std::string(value);
	}
	else if constexpr (std::is_same<From,bool>::value)
	{
		return value ? "true" : "false";
	}
	else if constexpr (std::is_arithmetic<From>::value)
	{
		return conv.convert(value);
	}
	else []<bool flag = false>()
	{
		static_assert(flag, "no string conversion");
	}();
}

}

#endif
