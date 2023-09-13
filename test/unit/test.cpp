#include "util/test.h"

#include <iostream>

#include "chart/main/version.h"

class application : public test::application
{
public:
	explicit application(std::list<std::string> &&argv) :
	    test::application(std::move(argv))
	{
		args.add_option('v',
		    "prints version",
		    []()
		    {
			    std::cout
			        << static_cast<std::string>(Vizzu::Main::version)
			        << "\n";
			    std::exit(EXIT_SUCCESS);
		    });
	}
};

int main(int argc, char *argv[])
{
	return application({argv + 1, argv + argc}).run();
}
