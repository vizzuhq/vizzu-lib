#ifndef TEST_ABSTRACT_CASE
#define TEST_ABSTRACT_CASE

#include <string>
#include <functional>
#include <iostream>
#include <chrono>
#include <map>

#include "src_location.h"

namespace test {

typedef std::function<void()> runable;

class case_type
{
public:

    case_type(std::string suite_name,
                    std::string case_name,
                    runable runner,
                    src_location location)
        : suite_name(suite_name),
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

    void fail(src_location location, const std::string &message)
    {
        if (!error_messages.contains(location))
            error_messages.insert({ location, message });
    }

    std::string full_name() const {
        return "[" + suite_name + "] " + case_name;
    }

    std::string file_name() const {
        return std::string(location.get_file_name());
    }

    operator bool() const { return error_messages.empty(); }

private:

    std::string suite_name;
    std::string case_name;
    runable runner;
    src_location location;
    std::map<src_location, std::string> error_messages;

    void run_safely() noexcept
    {
        try { runner(); }

        catch (std::exception &e) {
            fail(location, "exception thrown: "
				+ std::string(e.what()));
        }
		catch (...) {
            fail(location, "unknown exception thrown");
		}
    }

    void print_start() const {
        std::cout << ansi::fg_yellow << "[ RUN... ] "
		          << ansi::reset << full_name() << "\r";
		std::cout.flush();
    }

    void print_summary(auto duration) const
    {
        using namespace std::chrono;

        std::cout
            << (error_messages.empty()
                ? std::string(ansi::fg_green) + "[     OK ] "
                : std::string(ansi::fg_red) + "[ FAILED ] ")
            << ansi::reset << full_name() << " ("
            << (duration_cast<milliseconds>(duration).count()) << " ms)\n";

		for (auto error: error_messages)
            std::cerr << error.first.error_prefix()
			          << "error: " << error.second << "\n";
    }
};

}

#endif
