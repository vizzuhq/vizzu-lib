#ifndef CONV_TOSTRING
#define CONV_TOSTRING

#include <sstream>
#include <string>

namespace Conv
{

template<typename T>
concept ostreamable = requires(std::ostream &stream, T p) { stream << p; };

template <typename From>
std::string toString(const From &value)
{
	if constexpr (std::is_constructible_v<std::string, From>)
	{
		return std::string(value);
	}
	else if constexpr (std::is_same_v<From,bool>)
	{
		return value ? "true" : "false";
	}
	else if constexpr (ostreamable<From>)
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
