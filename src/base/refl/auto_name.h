#ifndef VIZZU_REFL_AUTO_NAME_H
#define VIZZU_REFL_AUTO_NAME_H

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>

namespace Refl::Name
{

template <class E, auto v> consteval auto name()
{
#ifdef _MSC_VER
	constexpr std::string_view sv = __FUNCSIG__;
	constexpr auto last = sv.find_last_not_of(" }>)(", sv.size() - 6);
#else
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
	              res.length() > 0
	              && (res[0] < '0' || res[0] > '9')) {
		std::array<char, res.size()> arr{};
		std::ranges::copy(res, arr.begin());
		return arr;
	}
	else {
		return std::array<char, 0>{};
	}
}
}

#endif
