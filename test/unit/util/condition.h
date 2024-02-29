#ifndef TEST_CONDITION
#define TEST_CONDITION

#include <utility>

#include "collection.h"
#include "to_string.h"

namespace test
{

struct check_t;

template <typename exception = std::exception> struct throws_t;

template <typename T> struct decomposer
{
public:
	friend check_t;

	template <class U> void operator==(const U &ref) const
	{
		if constexpr (requires { ref == value; }) {
			return evaluate(value == ref, "==", ref);
		}
		else if constexpr (std::ranges::range<T>) {
			auto &&[lhs, rhs] = std::ranges::mismatch(value, ref);
			return evaluate(lhs == std::end(value)
			                    && rhs == std::end(ref),
			    "==",
			    ref);
		}
		else if constexpr (std::is_convertible_v<U, T>) {
			return *this == static_cast<T>(ref);
		}
		else {
			static_assert(
			    requires { ref == value; },
			    "Cannot compare types");
		}
	}

	template <class U> void operator!=(const U &ref) const
	{
		if constexpr (requires { ref != value; }) {
			return evaluate(value != ref, "!=", ref);
		}
		else if constexpr (std::ranges::range<T>) {
			if (std::ranges::size(value) != std::ranges::size(ref))
				return;
			auto &&[lhs, rhs] = std::ranges::mismatch(value, ref);
			return evaluate(lhs != std::end(value), "!=", ref);
		}
		else if constexpr (std::is_convertible_v<U, T>) {
			return *this != static_cast<T>(ref);
		}
		else {
			static_assert(
			    requires { ref != value; },
			    "Cannot compare types");
		}
	}

	void operator<=(const auto &ref) const
	{
		return evaluate(value <= ref, "<=", ref);
	}
	void operator>=(const auto &ref) const
	{
		return evaluate(value >= ref, ">=", ref);
	}
	void operator<(const auto &ref) const
	{
		return evaluate(value < ref, "<", ref);
	}
	void operator>(const auto &ref) const
	{
		return evaluate(value > ref, ">", ref);
	}

private:
	const T &value;
	src_location location;
	void (*throw_error)(const std::string &, src_location);

	decomposer(const T &value,
	    src_location loc,
	    void (*throw_error)(const std::string &,
	        src_location) = nullptr) :
	    value(value),
	    location(loc),
	    throw_error(throw_error)
	{}

	void
	evaluate(bool condition, const char *op, const auto &ref) const
	{
		using details::to_debug_string;

		if (!condition) {
			if (throw_error) {
				throw_error(
				    std::string("comparison failed\n") + "\t(actual) "
				        + to_debug_string(value) + " " + op + " "
				        + to_debug_string(ref) + " (expected)",
				    location);
			}

			collection::instance().running_test()->fail(location,
			    std::string("Check comparison failed\n")
			        + "\t(actual) " + to_debug_string(value) + " "
			        + op + " " + to_debug_string(ref)
			        + " (expected)");
		}
	}
};

inline namespace consts
{
struct impl_check_t
{
	test::check_t operator()(src_location loc = src_location()) const;
};
struct assert_t
{};
struct skip_t
{};
template <class exception> struct impl_throws_t
{
	test::throws_t<exception> operator()(
	    src_location loc = src_location()) const;
};
}

struct check_t
{
	explicit check_t(src_location loc = src_location(),
	    void (*err)(const std::string &,
	        src_location) = nullptr) noexcept :
	    location(loc),
	    throw_error(err)
	{
		collection::instance().running_test()->set_latest_location(
		    loc);
	}

	explicit(false) check_t(consts::impl_check_t,
	    src_location loc = src_location()) noexcept :
	    check_t(loc)
	{}

	explicit(false) check_t(consts::assert_t,
	    src_location loc = src_location()) noexcept :
	    check_t(
	        loc,
	        +[](const std::string &msg, src_location loc)
	        {
		        throw assertion_error(msg, loc);
	        })
	{}

	explicit(false) check_t(consts::skip_t,
	    src_location loc = src_location()) noexcept :
	    check_t(
	        loc,
	        +[](const std::string &msg, src_location loc)
	        {
		        throw skip_error(msg, loc);
	        })
	{}

	[[nodiscard]] auto operator<<(const auto &value) const
	{
		return decomposer(value, location, throw_error);
	}

	src_location location;
	void (*throw_error)(const std::string &, src_location) = nullptr;
};

struct expected_bool_t
{
	bool value;
	std::string_view msg;

	constexpr explicit(false)
	    expected_bool_t(std::string_view msg, bool value = true) :
	    value(value),
	    msg(msg)
	{}
};

constexpr expected_bool_t operator""_is_true(const char *msg,
    std::size_t len)
{
	return expected_bool_t{{msg, len}};
}

constexpr expected_bool_t operator""_is_false(const char *msg,
    std::size_t len)
{
	return expected_bool_t{{msg, len}, false};
}

struct bool_check_t : check_t
{
	bool value;

	void operator==(expected_bool_t exp) const
	{
		using details::to_debug_string;

		if (value != exp.value) {
			if (throw_error) {
				throw_error(std::string("expectation failed\n")
				                + "\t\"" + std::string{exp.msg}
				                + "\" is "
				                + (exp.value ? "false" : "true"),
				    location);
			}

			collection::instance().running_test()->fail(location,
			    std::string("Check expectation failed\n") + "\t\""
			        + std::string{exp.msg} + "\" is "
			        + (exp.value ? "false" : "true"));
		}
	}
};

template <typename exception> struct throws_t
{
	explicit throws_t(src_location loc = src_location()) :
	    location(loc)
	{
		collection::instance().running_test()->set_latest_location(
		    loc);
	}

	explicit(false) throws_t(consts::impl_throws_t<exception>,
	    src_location loc = src_location()) :
	    throws_t(loc)
	{}

	auto &operator<<(const auto &f) const
	{
		try {
			f();
			collection::instance().running_test()->fail(location,
			    "Exception expected but not thrown");
		}
		catch (const assertion_error &) {
			throw;
		}
		catch (const skip_error &) {
			throw;
		}
		catch (const exception &) {
		}
		catch (...) {
			collection::instance().running_test()->fail(location,
			    "Exception thrown, but not the expected type");
		}
		return *this;
	}

	src_location location;
};

template <typename exception>
throws_t(consts::impl_throws_t<exception>,
    src_location loc = src_location()) -> throws_t<exception>;

[[nodiscard]] inline auto operator->*(check_t &&check,
    const auto &value)
{
	return check << value;
}
[[nodiscard]] inline bool_check_t operator->*(check_t &&check,
    bool value)
{
	return bool_check_t{check, value};
}

inline auto operator->*(throws_t<std::runtime_error> &&throws,
    const auto &value)
{
	return throws << value;
}

inline namespace consts
{

[[nodiscard]] inline test::check_t impl_check_t::operator()(
    src_location loc) const
{
	return test::check_t{loc};
}

template <class exception>
[[nodiscard]] inline test::throws_t<exception>
impl_throws_t<exception>::operator()(src_location loc) const
{
	return test::throws_t<exception>{loc};
}

static inline constexpr impl_check_t check{};
static inline constexpr assert_t assert{};
static inline constexpr skip_t skip{};

static inline constexpr impl_throws_t<std::runtime_error> throw_{};
template <class exception = std::exception>
static inline constexpr impl_throws_t<exception> throws{};
}
}

#endif
