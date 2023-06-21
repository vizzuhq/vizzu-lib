#ifndef TEST_SRC_LOCATION
#define TEST_SRC_LOCATION

#include <string>
#include <string_view>

#ifndef TEST_MOCK_SOURCE_LOCATION
#if __has_include(<source_location>) and defined(__cpp_lib_source_location)
#include <source_location>
typedef std::source_location source_location;
#elif __has_include(<experimental/source_location>)
#include <experimental/source_location>
typedef std::experimental::source_location source_location;
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
	std::string get_file_name() const { return "unknown"; };
	std::size_t get_line() const { return 0; };
	std::string error_prefix() const { return ""; }
	bool operator==(const src_location&) const { return true; }
	bool operator<(const src_location&) const { return false; }
};
#else

class src_location
{
public:
	src_location(const source_location &location =
	                 source_location::current())
	{
		file_name = std::string(location.file_name());
		line = location.line();
	}

	std::string error_prefix() const
	{
		return format_error(file_name, line);
	}

	std::string get_file_name() const { return file_name; };
	std::size_t get_line() const { return line; };

	bool operator==(const src_location&) const = default;
	bool operator<(const src_location& oth) const {
		return file_name < oth.file_name || (file_name == file_name && line < oth.line);
	}

private:
	std::string file_name;
	std::size_t line;
};

#endif

}

#endif
