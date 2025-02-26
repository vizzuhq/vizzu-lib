#ifndef TEST_TO_STRING
#define TEST_TO_STRING

#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

namespace test::details
{

template <typename T>
concept streamable =
    requires(std::ostream &stream, T p) { stream << p; };

template <typename from>
std::string to_debug_string(const from &value)
{
	if constexpr (std::is_same_v<from, std::string>
	              || std::is_same_v<from, const char *>) {
		return std::string("\"") + value + "\"";
	}
	else if constexpr (streamable<from>) {
		auto ss = std::stringstream{};
		ss << value;
		return ss.str();
	}
	else if constexpr (std::is_constructible_v<std::string, from>) {
		return std::string(value);
	}
	else if constexpr (requires {
		                   std::visit([](const auto &) {}, value);
	                   }) {
		return std::visit(
		    [](const auto &v)
		    {
			    return to_debug_string(v);
		    },
		    value);
	}
	else if constexpr (std::ranges::range<from>) {
		auto ss = std::stringstream{};
		ss << "[";
		bool first = true;
		for (auto &&v : value) {
			if (!first)
				ss << ", ";
			else
				first = false;
			ss << to_debug_string(v);
		}
		ss << "]";
		return ss.str();
	}
	else if constexpr (requires { std::tuple_size<from>::value; }) {
		auto ss = std::stringstream{};
		ss << "(";
		bool first = true;
		std::apply(
		    [&](auto &&...v)
		    {
			    ((ss << (first ? "" : ", ") << to_debug_string(v),
			         first = false),
			        ...);
		    },
		    value);
		ss << ")";
		return ss.str();
	}
	else
		return "<No string conversion>";
}

}

#endif
