#ifndef TEST_COLLECTOR
#define TEST_COLLECTOR

#include <chrono>
#include <regex>

#include "ansi.h"
#include "case_registry.h"
#include "suite_proxy.h"

namespace test
{

struct all
{};

class collection : public case_registry
{
public:
	static collection &instance()
	{
		static collection instance;
		return instance;
	}

	std::size_t run(const all &) { return run(""); }

	std::size_t run(const std::string &regex)
	{
		std::regex re(".*" + regex + ".*");

		auto stats = run_if(
		    [&](auto test)
		    {
			    return regex.empty()
			        || std::regex_match(test.full_name(), re);
		    });

		print_summary(stats);

		return stats.failed;
	}

	std::size_t run_file(const std::string &file_name)
	{
		return run_if(
		    [&](auto test)
		    {
			    return test.file_name() == file_name;
		    })
		    .failed;
	}

	void list() const
	{
		std::string act_file;

		for (const auto &act_case : cases) {
			if (act_case.file_name() != act_file) {
				act_file = act_case.file_name();
				std::cout << act_file << ":\n";
			}
			std::cout << "\t" << act_case.full_name() << "\n";
		}
	}

	static suite_proxy add_suite(std::string_view name) noexcept
	{
		return {name, instance()};
	}

	case_type *running_test() { return running_case; }

private:
	struct statistics
	{
		std::size_t run{};
		std::size_t failed{};
		statistics() = default;
	};

	case_type *running_case{};

	statistics run_if(
	    const std::function<bool(const case_type &)> &condition)
	{
		statistics stats;

		for (auto &act_case : cases) {
			if (condition(act_case)) {
				stats.run++;
				running_case = &act_case;
				act_case();
				if (!act_case) stats.failed++;
			}
		}
		return stats;
	}

	void print_summary(statistics stats)
	{
		std::cout << "\n"
		          << "all tests:    " << cases.size() << "\n"
		          << "tests run:    " << stats.run << "\n"
		          << "tests failed: " << stats.failed << "\n";

		if (stats.failed > 0)
			for (auto &act_case : cases)
				if (!act_case)
					std::cout << "\t" << act_case.full_name() << "\n";

		std::cout << "\n";
	}
};

inline suite_proxy operator""_suite(const char *s,
    std::size_t n) noexcept
{
	return collection::add_suite(std::string_view{s, n});
}

}

#endif
