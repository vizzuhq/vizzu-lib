#ifndef TEST_CONDITION
#define TEST_CONDITION

#include <utility>

#include "collection.h"
#include "to_string.h"

namespace test
{

struct check;

template <typename T> struct decomposer
{
public:
	friend check;

	bool operator==(const auto &ref) const
	{
		return evaluate(value == ref, ref);
	}
	bool operator<=(const auto &ref) const
	{
		return evaluate(value <= ref, ref);
	}
	bool operator>=(const auto &ref) const
	{
		return evaluate(value >= ref, ref);
	}
	bool operator<(const auto &ref) const
	{
		return evaluate(value < ref, ref);
	}
	bool operator>(const auto &ref) const
	{
		return evaluate(value > ref, ref);
	}

private:
	const T &value;
	src_location location;

	decomposer(const T &value, src_location loc) :
	    value(value),
	    location(std::move(loc))
	{}

	[[nodiscard]] bool evaluate(bool condition, const auto &ref) const
	{
		using namespace details;
		if (!condition) {
			collection::instance().running_test()->fail(location,
			    std::string("comparison failed\n")
			        + "\tactual:   " + to_debug_string(value) + "\n"
			        + "\texpected: " + to_debug_string(ref));
		}
		return condition;
	}
};

struct check
{
	explicit check(src_location loc = src_location()) noexcept :
	    location(std::move(loc))
	{}

	auto operator<<(const auto &value) const
	{
		return decomposer(value, location);
	}

	src_location location;
};

struct assertion
{
	explicit assertion(src_location loc = src_location()) :
	    location(std::move(loc))
	{}

	auto &operator<<(const auto &condition) const
	{
		if (!condition) {
			collection::instance().running_test()->fail(location,
			    "assertion failed");
		}
		return *this;
	}

	src_location location;
};

struct fail
{
	explicit fail(const src_location &loc = src_location())
	{
		collection::instance().running_test()->fail(loc,
		    "assertion failed");
	}
};

template <typename exception = std::exception> struct throws
{
	explicit throws(src_location loc = src_location()) :
	    location(std::move(loc))
	{}

	auto &operator<<(const auto &f) const
	{
		try {
			f();
			collection::instance().running_test()->fail(location,
			    "exception expected but not thrown");
		}
		catch (const exception &) {
		}
		return *this;
	}

	src_location location;
};

}

#endif
