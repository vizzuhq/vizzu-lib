#ifndef VIZZU_REFL_AUTO_ENUM_H
#define VIZZU_REFL_AUTO_ENUM_H

#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>

#include "auto_name.h"

namespace Refl
{

[[maybe_unused]] [[noreturn]] static void error_str(std::size_t value,
    std::size_t size)
{
	throw std::logic_error(
	    "not an enum value: '" + std::to_string(value)
	    + "', valid values: 0.." + std::to_string(size - 1));
}

[[maybe_unused]] [[noreturn]] static void
error_str(std::string_view name, std::string_view code)
{
	throw std::logic_error("not an enum name: '" + std::string(name)
	                       + "', valid name: " + std::string(code));
}

namespace Detail
{
template <class E, std::size_t C = 0> consteval std::size_t count()
{
	if constexpr (Name::name<E,
	                  static_cast<E>(
	                      static_cast<std::underlying_type_t<E>>(
	                          C))>()
	                  .empty())
		return C;
	else
		return count<E, C + 1>();
}

template <class E>
concept UniqueNames = requires {
	static_cast<std::string_view>(unique_enum_names(E{}));
};

template <class E, std::size_t... Ix>
consteval auto whole_array(std::index_sequence<Ix...> = {})
{
	if constexpr (UniqueNames<E>) {
		constexpr std::string_view pre_res = unique_enum_names(E{});
		std::array<char, std::size(pre_res)> res{};
		const auto *v = pre_res.data();
		for (auto &r : res) r = *v++;
		return res;
	}
	else {
		std::array<char,
		    (Name::name<E, static_cast<E>(Ix)>().size() + ...
		        + (sizeof...(Ix) - 1))>
		    res{};
		auto resp = res.begin();

		auto copy = [&](auto arr)
		{
			for (auto c : arr) *resp++ = c;
			if (resp != res.end()) *resp++ = ',';
		};
		(copy(Name::name<E, static_cast<E>(Ix)>()), ...);
		return res;
	}
}
}

template <class E>
constexpr std::array enum_name_holder = Detail::whole_array<E>(
    std::make_index_sequence<Detail::count<E>()>{});

template <class E, std::size_t... Ix>
consteval auto get_names(std::index_sequence<Ix...> = {})
{
	static_assert(Detail::count<E>() > 0);
	constexpr std::string_view str{enum_name_holder<E>.data(),
	    enum_name_holder<E>.size()};
	constexpr auto c = std::count(str.begin(), str.end(), ',') + 1;
	if constexpr (c == sizeof...(Ix)) {
		auto whole_str = str;
		std::array<std::string_view, c> res{};
		for (auto &resp : res) {
			auto split = whole_str.find(',');
			resp = whole_str.substr(0, split);
			whole_str = whole_str.substr(split + 1);
		}
		return res;
	}
	else {
		return get_names<E>(std::make_index_sequence<c>{});
	}
}

template <class E> constexpr std::array enum_names = get_names<E>();

template <class E> std::string enum_name(E name)
{
	constexpr auto n = std::size(enum_names<E>);
	if (static_cast<std::size_t>(name) < n) {
		auto sv = enum_names<E>[static_cast<std::size_t>(name)];
		return {sv.data(), sv.size()};
	}
	error_str(static_cast<std::size_t>(name), n);
}

template <class E> constexpr E get_enum(std::string_view data)
{
	std::size_t ix{};
	for (auto v : enum_names<E>) {
		if (v == data) break;
		++ix;
	}
	if (ix == std::size(enum_names<E>))
		error_str(data,
		    {enum_name_holder<E>.data(), enum_name_holder<E>.size()});
	return static_cast<E>(ix);
}

template <class E, class V>
struct EnumArray : std::array<V, std::size(enum_names<E>)>
{
	using base_array = std::array<V, std::size(enum_names<E>)>;
	[[nodiscard]] constexpr typename base_array::reference operator[](
	    E value) noexcept
	{
		return base_array::operator[](
		    static_cast<std::size_t>(value));
	}

	[[nodiscard]] constexpr typename base_array::const_reference
	operator[](E value) const noexcept
	{
		return base_array::operator[](
		    static_cast<std::size_t>(value));
	}

	[[nodiscard]] constexpr typename base_array::reference at(E value)
	{
		return base_array::at(static_cast<std::size_t>(value));
	}

	[[nodiscard]] constexpr typename base_array::const_reference at(
	    E value) const
	{
		return base_array::at(static_cast<std::size_t>(value));
	}
};

}

#endif