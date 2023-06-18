#ifndef TEST_APPLICATION
#define TEST_APPLICATION

#include "arguments.h"
#include "collection.h"

namespace test
{

class application
{
public:
	application(int argc, char *argv[]) :
	    args(argc, argv, {"-a"}),
	    exit_code(EXIT_SUCCESS)
	{
		args.add_option('h',
			"prints help",
			[this]()
			{
				args.print_help();
			});

		args.add_option('l',
			"lists tests",
			[]()
			{
				collection::instance().list();
			});

		args.add_option('a',
			"runs all tests",
			[this]()
			{
				exit_code = collection::instance().run(all{});
			});

		args.add_option('f',
			"runs tests from file",
			[this](const std::string &file)
			{
				exit_code = collection::instance().run_file(file);
			});

		args.add_option('t',
			"runs tests matching regexp",
			[this](const std::string &regex_pattern)
			{
				exit_code = collection::instance().run(regex_pattern);
			});
	}

	int run()
	{
		try {
			args.process();
		}
		catch (const std::exception &e) {
			std::cerr << "Fatal error: " << e.what() << "\n\n";
			exit_code = EXIT_FAILURE;
		}
		catch (...) {
			std::cerr << "Fatal error: unhandled exception\n\n";
			exit_code = EXIT_FAILURE;
		}
		return exit_code;
	}

protected:
	arguments args;
	int exit_code;
};

}

#endif
