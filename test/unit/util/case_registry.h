#ifndef TEST_CASE_REGISTRY
#define TEST_CASE_REGISTRY

#include <vector>

#include "base/io/log.h"

#include "case.h"

namespace test
{

class suite_proxy;

class case_registry
{
protected:
	friend suite_proxy;

	std::vector<case_type> cases;

	void add_record(std::string_view suite_name,
	    std::string_view case_name,
	    runnable &&runner,
	    src_location location) noexcept
	{
		try {
			cases.emplace_back(suite_name,
			    case_name,
			    std::move(runner),
			    location);
		}
		catch (...) {
			IO::log() << "Add record failed at static time: "
			             "["
			          << suite_name << "] " << case_name;
		}
	}
};

}

#endif
