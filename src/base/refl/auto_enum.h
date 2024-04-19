#ifndef VIZZU_REFL_AUTO_ENUM_H
#define VIZZU_REFL_AUTO_ENUM_H

#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

#include "auto_name.h"

namespace Refl
{

[[maybe_unused]] [[noreturn]] static void
error_str(std::intptr_t value, std::intptr_t from, std::intptr_t to)
{
	throw std::logic_error(
	    "not an enum value: '" + std::to_string(value)
	    + "', valid values: " + std::to_string(from) + ".."
	    + std::to_string(to - 1));
}

[[maybe_unused]] [[noreturn]] static void
error_str(std::string_view name, std::string_view code)
{
	throw std::logic_error("not an enum name: '" + std::string(name)
	                       + "', valid name: " + std::string(code));
}

namespace Detail
{
template <class T> using real_t = std::underlying_type_t<T>;

template <class E, real_t<E> From, real_t<E> To>
consteval std::pair<real_t<E>, real_t<E>> from_to()
{
	if constexpr (std::is_signed_v<real_t<E>>
	              && !Name::name<E, static_cast<E>(From - 1)>()
	                      .empty())
		return from_to<E, From - 1, To>();
	else if constexpr (!Name::name<E, static_cast<E>(To)>().empty())
		return from_to<E, From, To + 1>();
	else
		return {From, To};
}

template <class E> consteval real_t<E> count()
{
	auto [from, to] = from_to<E, 0, 0>();
	return static_cast<real_t<E>>(to - from);
}

template <class E>
concept UniqueNames = requires {
	static_cast<std::string_view>(unique_enum_names(E{}));
};

template <class E, real_t<E>... Ix>
consteval auto whole_array(
    std::integer_sequence<real_t<E>, Ix...> = {})
{
	if constexpr (UniqueNames<E>) {
		constexpr std::string_view pre_res = unique_enum_names(E{});
		std::array<char, std::size(pre_res)> res{};
		std::ranges::copy(pre_res, res.begin());
		return res;
	}
	else {
		constexpr auto first = Detail::from_to<E, 0, 0>().first;
		std::array<char,
		    (Name::name<E, static_cast<E>(Ix + first)>().size() + ...
		        + (sizeof...(Ix) - 1))>
		    res{};
		auto copy = [&res, resp = res.begin()](auto arr) mutable
		{
			for (auto c : arr) *resp++ = c;
			if (resp != res.end()) *resp++ = ',';
		};
		(copy(Name::name<E, static_cast<E>(Ix + first)>()), ...);
		return res;
	}
}
}

template <class E>
constexpr std::array enum_name_holder = Detail::whole_array<E>(
    std::make_integer_sequence<Detail::real_t<E>,
        Detail::count<E>()>{});

template <class E, std::size_t... Ix>
consteval auto get_names(std::index_sequence<Ix...> = {})
{
	static_assert(!enum_name_holder<E>.empty());
	if constexpr (constexpr auto c =
	                  std::count(std::begin(enum_name_holder<E>),
	                      std::end(enum_name_holder<E>),
	                      ',')
	                  + 1;
	              c == sizeof...(Ix)) {
		std::string_view whole_str{enum_name_holder<E>.data(),
		    enum_name_holder<E>.size()};
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

template <class Type = std::string_view, class E>
Type enum_name(E name)
{
	constexpr auto first = Detail::from_to<E, 0, 0>().first;
	constexpr auto n = std::size(enum_names<E>);
	if (static_cast<std::size_t>(
	        static_cast<Detail::real_t<E>>(name) - first)
	    < n) {
		auto &&sv = enum_names<E>[static_cast<Detail::real_t<E>>(name)
		                          - first];
		return {sv.data(), sv.size()};
	}
	error_str(static_cast<std::intptr_t>(name),
	    static_cast<std::intptr_t>(first),
	    static_cast<std::intptr_t>(n - first));
}

template <class E> constexpr E get_enum(const std::string_view &data)
{
	constexpr auto first = Detail::from_to<E, 0, 0>().first;
	Detail::real_t<E> ix{};
	for (auto v : enum_names<E>) {
		if (v == data) break;
		++ix;
	}
	if (static_cast<std::size_t>(ix) == std::size(enum_names<E>))
		error_str(data,
		    {enum_name_holder<E>.data(), enum_name_holder<E>.size()});
	return static_cast<E>(ix + first);
}

template <class E, class V>
struct EnumArray : std::array<V, std::size(enum_names<E>)>
{
	using base_array = std::array<V, std::size(enum_names<E>)>;
	[[nodiscard]] constexpr V &operator[](E value) noexcept
	{
		return base_array::operator[](
		    static_cast<std::size_t>(value));
	}

	[[nodiscard]] constexpr const V &operator[](
	    E value) const noexcept
	{
		return base_array::operator[](
		    static_cast<std::size_t>(value));
	}

	[[nodiscard]] constexpr V &at(E value)
	{
		return base_array::at(static_cast<std::size_t>(value));
	}

	[[nodiscard]] constexpr const V &at(E value) const
	{
		return base_array::at(static_cast<std::size_t>(value));
	}
};

template <class E, class... Args>
    requires(std::is_enum_v<E>
             && sizeof...(Args) == Detail::count<E>()
             && Detail::from_to<E, 0, 0>().first == 0)
struct EnumVariant : std::variant<Args...>
{
	using base_variant = std::variant<Args...>;
	using base_variant::base_variant;

	[[nodiscard]] constexpr operator E() const noexcept // NOLINT
	{
		return static_cast<E>(base_variant::index());
	}

	using base_variant::emplace;

	template <E value, class... Constr>
	decltype(auto) emplace(Constr &&...args)
	{
		return base_variant::template emplace<
		    static_cast<std::size_t>(value)>(
		    std::forward<Constr>(args)...);
	}
};

template <auto E, class... Args>
constexpr decltype(auto) unsafe_get(
    EnumVariant<decltype(E), Args...> const &e)
{
	return *std::get_if<static_cast<std::size_t>(E)>(&e);
}

template <auto E, class... Args>
constexpr decltype(auto) unsafe_get(
    EnumVariant<decltype(E), Args...> &e)
{
	return *std::get_if<static_cast<std::size_t>(E)>(&e);
}

template <class T, class E, class... Args>
constexpr decltype(auto) unsafe_get(EnumVariant<E, Args...> const &e)
{
	return *std::get_if<T>(&e);
}

template <class T, class E, class... Args>
constexpr decltype(auto) unsafe_get(EnumVariant<E, Args...> &e)
{
	return *std::get_if<T>(&e);
}

template <auto E, class... Args>
constexpr decltype(auto) get_if(
    EnumVariant<decltype(E), Args...> const *e)
{
	return std::get_if<static_cast<std::size_t>(E)>(e);
}

template <auto E, class... Args>
constexpr decltype(auto) get_if(EnumVariant<decltype(E), Args...> *e)
{
	return std::get_if<static_cast<std::size_t>(E)>(e);
}

}

#endif