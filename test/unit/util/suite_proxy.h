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

template <class, class, class...> struct multi_input;

template <class T = void *>
    requires(std::is_default_constructible_v<T>)
struct input
{
	explicit(false) input(T &&,
	    std::string_view test_desc = {},
	    src_location loc = test::src_location{}) :
	    loc(loc),
	    test_desc(test_desc)
	{}

	template <class U>
	explicit input(input<U> const &oth) :
	    loc(oth.loc),
	    test_desc(oth.test_desc)
	{}

	template <class F>
	    requires(std::is_default_constructible_v<F>
	             && std::is_invocable_v<F, std::invoke_result_t<T>>)
	multi_input<std::index_sequence<0>, F, T> operator<=>(F) const
	{
		return {input<>{*this}};
	}

	src_location loc;
	std::string_view test_desc;
};

template <class T> input(T &&, std::string_view = {}) -> input<T>;

template <std::size_t... Ix, class L, class... Ts>
    requires(sizeof...(Ts) > 0 && sizeof...(Ix) == sizeof...(Ts)
             && !std::is_void_v<L>)
struct multi_input<std::index_sequence<Ix...>, L, Ts...>
{
	std::array<input<>, sizeof...(Ts)> locations;
};

template <std::size_t... Ix, class... Ts>
    requires(sizeof...(Ts) > 0 && sizeof...(Ix) == sizeof...(Ts))
struct multi_input<std::index_sequence<Ix...>, void, Ts...>
{
	template <class F>
	    requires(std::is_default_constructible_v<F> && ...
	             && std::is_invocable_v<F, std::invoke_result_t<Ts>>)
	multi_input<std::index_sequence<Ix...>, F, Ts...> operator<=>(
	    F) const
	{
		return {locations};
	}

	std::array<input<>, sizeof...(Ts)> locations;
};

template <class T, class U>
multi_input<std::index_sequence_for<U, T>, void, T, U>
operator+(const input<T> &a, const input<U> &b)
{
	return {input<>{a}, input<>{b}};
}

template <std::size_t... Ix, class... T, class U>
multi_input<std::index_sequence<Ix..., sizeof...(Ix)>, void, T..., U>
operator+(
    const multi_input<std::index_sequence<Ix...>, void, T...> &a,
    const input<U> &b)
{
	return {a.locations[Ix]..., input<>{b}};
}

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

	struct case_name_proxy
	{
		std::string_view case_name;
		src_location location;

		explicit(false) case_name_proxy(std::string_view case_name,
		    src_location location = src_location()) :
		    case_name(case_name),
		    location(location)
		{}

		template <std::size_t N>
		explicit(false) case_name_proxy(const char (&a)[N],
		    src_location location = src_location()) :
		    case_name_proxy({a, N - 1}, location)
		{}
	};

	struct case_proxy : case_name_proxy
	{
		suite_proxy &suite;

		template <std::size_t... Ix, class Lambda, class... Inputs>
		suite_proxy &operator|(
		    multi_input<std::index_sequence<Ix...>, Lambda, Inputs...>
		        &&i)
		{

			static const auto s = suite.parent.cases.size();
			static auto &p = suite.parent;
			static const auto mi = i;
			static const auto set = []<std::size_t I, class In>
			{
				p.cases[s].set_suffix(
				    "\n\t  on "
				    + std::string{mi.locations[I].loc.error_prefix()}
				    + (mi.locations[I].test_desc.data()
				            ? std::string{mi.locations[I].test_desc}
				            : std::to_string(I) + ". input."));

				Lambda{}(In{}());

				p.cases[s].set_suffix("");
			};

			suite.add_case(
			    case_name,
			    +[]
			    {
				    (set.template operator()<Ix, Inputs>(), ...);
			    },
			    location);

			return suite;
		}

		template <class Lambda>
		    requires(std::is_trivially_constructible_v<Lambda>
		             && std::is_invocable_v<Lambda>
		             && !std::is_convertible_v<Lambda, void (*)()>)
		suite_proxy &operator|(Lambda &&) noexcept
		{
			suite.add_case(
			    case_name,
			    +[]
			    {
				    Lambda{}();
			    },
			    location);
			return suite;
		}

		suite_proxy &operator|(runnable &&test) noexcept
		{
			suite.add_case(case_name, test, location);
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

}

#endif
