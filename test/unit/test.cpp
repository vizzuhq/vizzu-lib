#include <iostream>
#include "base/test/test.h"

#include "chart/main/version.h"

class application : public test::application
{
public:
    application(int argc, char *argv[]) : test::application(argc, argv)
    {
        args.add_option('v', "prints version", []() {
            std::cout << (std::string)Vizzu::Main::version << "\n";
            std::exit(EXIT_SUCCESS);
        });
    }
};

int main(int argc, char *argv[])
{
    return application(argc, argv).run();
}
