#ifndef TEST_SUITE_PROXY
#define TEST_SUITE_PROXY

#include "case_registry.h"

#include <fstream>
#include <sstream>
#include <filesystem>

namespace test {

class suite_proxy;

typedef std::function<void(suite_proxy)> generator;
typedef std::function<void(const std::string &, const std::string &)> file_test;

class suite_proxy
{
public:
    suite_proxy(const std::string &name, case_registry &parent) :
        name(name),
        parent(parent)
    {}

    suite_proxy add_case(const std::string &case_name, const runable &test,
                         src_location location = src_location())
    {
        parent.add_record(case_type(name, case_name, test, location));
        return *this;
    }

    suite_proxy generate(generator gen)
    {
		gen(*this);
        return *this;
    }

	suite_proxy for_files(const std::string &path, file_test test)
	{
		if (!std::filesystem::exists(path)) return *this;

		for(auto& p: std::filesystem::directory_iterator(path))
			add_case(p.path().c_str(), [=]
			{
				std::string filename = p.path();
				std::ifstream in(filename.c_str());
				std::stringstream buffer;
				buffer << in.rdbuf();
				auto content = buffer.str();
				test(filename, content);
			});

		return *this;
	}

    std::string name;
    case_registry &parent;
};

}

#endif
