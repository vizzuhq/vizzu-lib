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
	    runnable &&test,
	    src_location location = src_location()) noexcept
	{
		parent.add_record(name, case_name, std::move(test), location);
		return *this;
	}

	struct case_name_proxy
	{
		std::string_view case_name;
		src_location location;
		explicit(false) case_name_proxy(std::string_view case_name,
		    src_location location = src_location()) :
		    case_name(case_name),
		    location(location)
		{}
	};

	struct case_proxy : case_name_proxy
	{
		suite_proxy &suite;

		suite_proxy &operator|(runnable &&test) noexcept
		{
			suite.add_case(case_name, std::move(test), location);
			return suite;
		}
	};

	case_proxy operator|(case_name_proxy &&case_) noexcept
	{
		return {case_, *this};
	}

	suite_proxy generate(const generator &gen)
	{
		gen(*this);
		return *this;
	}

	std::string_view name;
	case_registry &parent;
};

inline std::string_view operator"" _case(const char *name,
    size_t size)
{
	return {name, size};
}

}

#endif
