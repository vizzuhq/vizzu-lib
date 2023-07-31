#ifndef TEST_SUITE_PROXY
#define TEST_SUITE_PROXY

#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

#include "case_registry.h"

namespace test
{

class suite_proxy;

using generator = std::function<void(suite_proxy)>;
using file_test =
    std::function<void(const std::string &, const std::string &)>;

class suite_proxy
{
public:
	suite_proxy(std::string name, case_registry &parent) :
	    name(std::move(name)),
	    parent(parent)
	{}

	suite_proxy add_case(const std::string &case_name,
	    const runnable &test,
	    src_location location = src_location())
	{
		parent.add_record(
		    case_type(name, case_name, test, std::move(location)));
		return *this;
	}

	suite_proxy generate(const generator &gen)
	{
		gen(*this);
		return *this;
	}

	suite_proxy for_files(const std::string &path,
	    const file_test &test)
	{
		if (!std::filesystem::exists(path)) return *this;

		for (const auto &p : std::filesystem::directory_iterator(path))
			add_case(p.path().c_str(),
			    [=]
			    {
				    const std::string filename = p.path();
				    const std::ifstream in(filename.c_str());
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
