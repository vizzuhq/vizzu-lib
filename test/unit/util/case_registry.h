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

	void add_record(const case_type &new_case) noexcept
	{
		try {
			cases.push_back(new_case);
		}
		catch (...) {
			IO::log() << "Add record failed at static time: "
			          << new_case.full_name();
		}
	}
};

}

#endif
