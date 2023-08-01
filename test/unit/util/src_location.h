#ifndef TEST_SRC_LOCATION
#define TEST_SRC_LOCATION

#include <compare>
#include <string>
#include <string_view>

#ifndef TEST_MOCK_SOURCE_LOCATION
#if __has_include(<source_location>) and defined(__cpp_lib_source_location)
#include <source_location>
using source_location = std::source_location;
#elif __has_include(<experimental/source_location>)
#include <experimental/source_location>
using source_location = std::experimental::source_location;
#else
#define TEST_MOCK_SOURCE_LOCATION
#endif
#endif

namespace test
{

static inline std::string format_error(const std::string &file,
    std::size_t line)
{
	return file + ":" + std::to_string(line) + ": ";
}

#ifdef TEST_MOCK_SOURCE_LOCATION
struct src_location
{
	auto operator<=>(const src_location &) const { return 0 <=> 0; }
	std::string get_file_name() const { return "unknown"; };
	std::size_t get_line() const { return 0; };
	std::string error_prefix() const { return ""; }
};
#else

class src_location
{
public:
	explicit src_location(
	    const source_location &location = source_location::current())
	{
		file_name = std::string(location.file_name());
		line = location.line();
	}

	[[nodiscard]] std::string error_prefix() const
	{
		return format_error(file_name, line);
	}

	[[nodiscard]] std::string get_file_name() const
	{
		return file_name;
	};
	[[nodiscard]] std::size_t get_line() const { return line; };

	auto operator<=>(const src_location &other) const
	{
		auto file_cmp = file_name <=> other.file_name;
		return (file_cmp == decltype(file_cmp)::equal)
		         ? (line <=> other.line)
		         : file_cmp;
	}

private:
	std::string file_name;
	std::size_t line;
};

#endif

}

#endif
