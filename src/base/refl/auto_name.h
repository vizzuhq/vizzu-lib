#ifndef VIZZU_REFL_AUTO_NAME_H
#define VIZZU_REFL_AUTO_NAME_H

#include <algorithm>
#include <array>
#include <functional>
#include <ranges>
#include <string_view>

namespace Refl
{

inline namespace Declval
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

template <class T> extern T not_exists; // NOLINT

template <class T> consteval T declval()
{
	return std::forward<T>(not_exists<std::remove_reference_t<T>>);
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

namespace Impl
{
template <class Base, class Member>
static Base getBase(Member Base::*);
}

namespace Name
{
template <class T> struct Wrapper
{
	T t;
};

template <class T> Wrapper(T) -> Wrapper<T>;

template <class E, auto v> consteval auto name()
{
#ifdef _MSC_VER
	constexpr std::string_view sv = __FUNCSIG__;
	constexpr auto last = sv.find_last_not_of(" }>)(", sv.size() - 6);
#else
#if defined(__clang__)
	if constexpr (std::is_member_object_pointer_v<decltype(v)>)
		return name<void,
		    Wrapper{&std::invoke(v,
		        declval<decltype(Impl::getBase(v)) &>())}>();
#endif
	constexpr std::string_view sv = __PRETTY_FUNCTION__;
	constexpr auto last = sv.find_last_not_of(" }])");
#endif
	constexpr auto val =
	    sv.find_last_not_of("abcdefghijklmnopqrstuvwxyz"
	                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	                        "0123456789_",
	        last);
	if constexpr (constexpr std::string_view res =
	                  sv.substr(val + 1, last - val);
	              !res.empty() && (res[0] < '0' || res[0] > '9')) {
		std::array<char, res.size()> arr{};
		std::ranges::copy(res, arr.begin());
		return arr;
	}
	else
		return std::array<char, 0>{};
}

template <auto A>
constexpr static inline std::string_view in_data_name{std::data(A),
    std::size(A)};

}
}

#endif
