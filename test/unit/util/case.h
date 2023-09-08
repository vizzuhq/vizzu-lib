#ifndef TEST_ABSTRACT_CASE
#define TEST_ABSTRACT_CASE

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "src_location.h"

namespace test
{

using runnable = void (*)();

class case_type
{
public:
	case_type(std::string_view suite_name,
	    std::string_view case_name,
	    runnable runner,
	    src_location location) noexcept :
	    suite_name(suite_name),
	    case_name(case_name),
	    runner(runner),
	    location(location)
	{}

	void operator()()
	{
		using namespace std::chrono;

		print_start();
		auto start = steady_clock::now();

		run_safely();

		auto duration = steady_clock::now() - start;
		print_summary(duration);
	}

	void fail(const src_location &location,
	    const std::string &message)
	{
		if (!error_messages.contains(location))
			error_messages.insert({location, message});
	}

	[[nodiscard]] std::string full_name() const
	{
		return "[" + std::string{suite_name} + "] "
		     + std::string{case_name};
	}

	[[nodiscard]] std::string file_name() const
	{
		return location.get_file_name();
	}

	explicit operator bool() const { return error_messages.empty(); }

private:
	std::string_view suite_name;
	std::string_view case_name;
	runnable runner;
	src_location location;
	std::map<src_location, std::string> error_messages;

	void run_safely() noexcept
	{
		try {
			runner();
		}

		catch (std::exception &e) {
			fail(location,
			    "exception thrown: " + std::string(e.what()));
		}
		catch (...) {
			fail(location, "unknown exception thrown");
		}
	}

	void print_start() const
	{
		std::cout << ansi::fg_yellow << "[ RUN... ] " << ansi::reset
		          << this->full_name() << "\r";
		std::cout.flush();
	}

	void print_summary(auto duration) const
	{
		using namespace std::chrono;

		std::cout << (error_messages.empty()
		                  ? std::string(ansi::fg_green)
		                        + "[     OK ] "
		                  : std::string(ansi::fg_red) + "[ FAILED ] ")
		          << ansi::reset << full_name() << " ("
		          << (duration_cast<milliseconds>(duration).count())
		          << " ms)\n";

		for (const auto &error : error_messages)
			std::cerr << error.first.error_prefix()
			          << "error: " << error.second << "\n";
	}
};

}

#endif
