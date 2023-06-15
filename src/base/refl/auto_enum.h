#ifndef VIZZU_REFL_AUTO_ENUM_H
#define VIZZU_REFL_AUTO_ENUM_H

#include <array>
#include <string>
#include <string_view>

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

static constexpr std::size_t value(std::string_view name,
    const std::string_view *names,
    size_t size,
    std::string_view code)
{
	for (auto i = 0u; i < size; i++)
		if (name == names[i]) return i;
	throw std::logic_error("not an enum name: '" + std::string(name)
	                       + "', valid name: " + std::string(code));
}

template <class E, E v> consteval std::string_view name()
{
	// TODO make msvc to works with __FUNCSIG__
	constexpr std::string_view func{__PRETTY_FUNCTION__,
	    sizeof(__PRETTY_FUNCTION__) - 2};
	constexpr auto val =
	    func.find_last_not_of("abcdefghijklmnopqrstuvwxyz"
	                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	                          "0123456789_");
	constexpr std::string_view res = func.substr(val + 1);
	return res.length() > 0 && (res[0] < '0' || res[0] > '9')
	         ? res
	         : std::string_view{};
}

template <class E, std::size_t C = 0> consteval std::size_t count()
{
	if constexpr (name<E,
	                  static_cast<E>(
	                      static_cast<std::underlying_type_t<E>>(
	                          C))>()
	                  .empty())
		return C;
	else
		return count<E, C + 1>();
}

template <class E, std::size_t... Ix>
consteval auto whole_array(std::index_sequence<Ix...> = {})
{
	std::array<char,
	    (name<E, static_cast<E>(Ix)>().size() + ...
	        + (sizeof...(Ix) - 1))>
	    res{};
	auto resp = res.begin();
	for (auto sv : {name<E, static_cast<E>(Ix)>()...}) {
		for (auto c : sv) *resp++ = c;
		if (resp != res.end()) *resp++ = ',';
	}
	return res;
}

template <class E>
constexpr std::array enum_name_holder =
    whole_array<E>(std::make_index_sequence<count<E>()>{});

template <class E, std::size_t... Ix>
consteval std::array<std::string_view, count<E>()> get_names(
    std::index_sequence<Ix...> = {})
{
	std::array<std::string_view, count<E>()> res{};
	auto resp = res.data();
	auto where = enum_name_holder<E>.begin();
	for (auto sv : {name<E, static_cast<E>(Ix)>()...}) {
		*resp++ = std::string_view{where, sv.size()};
		where += sv.size();
		if (where != enum_name_holder<E>.end()) ++where;
	}
	return res;
}

template <class E>
constexpr std::array<std::string_view, count<E>()> enum_names =
    get_names<E>(std::make_index_sequence<count<E>()>{});

template <class E> std::string enum_name(E name)
{
	if (constexpr auto n = std::size(enum_names<E>);
	    static_cast<std::size_t>(name) < n) {
		auto sv = enum_names<E>[static_cast<std::size_t>(name)];
		return {sv.data(), sv.size()};
	}
	else
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
struct EnumArray : std::array<V, count<E>()>
{
	using base_array = std::array<V, count<E>()>;
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

#endif // VIZZU_REFL_AUTO_ENUM_H
