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

class suite_proxy
{
public:
	suite_proxy(std::string_view name, case_registry &parent) noexcept
	    :
	    name(name),
	    parent(parent)
	{}

	suite_proxy &add_case(std::string_view case_name,
	    runnable test,
	    src_location location = src_location()) noexcept
	{
		parent.add_record(name, case_name, test, location);
		return *this;
	}

	suite_proxy generate(const generator &gen)
	{
		gen(*this);
		return *this;
	}

	std::string_view name;
	case_registry &parent;
};

}

#endif
