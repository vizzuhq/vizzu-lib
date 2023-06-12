#ifndef TEST_TO_STRING
#define TEST_TO_STRING

#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

namespace test::details
{

template<typename T>
concept streamable = requires(std::ostream &stream, T p) { stream << p; };

template <typename from>
std::string to_debug_string(const from &value)
{
	if constexpr (
		std::is_same_v<from, std::string>
		|| std::is_same_v<from, const char*>
		)
	{
		return std::string("\"") + value + "\"";
	}
	else if constexpr (streamable<from>)
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
	else if constexpr (std::is_constructible_v<std::string, from>)
	{
		return std::string(value);
	}
	else
		return std::string("<No string conversion>");
}

}

#endif
