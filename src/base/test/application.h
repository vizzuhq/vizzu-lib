#ifndef TEST_APPLICATION
#define TEST_APPLICATION

#include "arguments.h"

namespace test
{

class application
{
public:
    application(int argc, char *argv[]) :
        args(argc, argv, { "-a" })
    {
        args.add_option('h', "prints help", [this]() {
            args.print_help();
            std::exit(EXIT_SUCCESS);
        });

        args.add_option('l', "lists tests", []() {
            collection::instance().list();
            std::exit(EXIT_SUCCESS);
        });

        args.add_option('a', "runs all tests", []() {
            std::exit(collection::instance().run(all{}));
        });

        args.add_option('f', "runs tests from file",
        [&](const std::string &file) {
            std::exit(collection::instance().run_file(file));
        });

        args.add_option('t', "runs tests matching regexp",
        [&](const std::string &regex_pattern) {
            std::exit(collection::instance().run(regex_pattern));
        });
    }

    int run()
    {
        try
        {
            args.process();
            return EXIT_SUCCESS;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Fatal error: " << e.what() << "\n\n";
            return EXIT_FAILURE;
        }
    }

private:
    arguments args;
};

}

#endif
