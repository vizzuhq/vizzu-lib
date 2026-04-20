#ifndef VIZZU_REFL_AUTO_ACCESS_H
#define VIZZU_REFL_AUTO_ACCESS_H

#include <map>
#include <ranges>
#include <string>
#include <string_view>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"
#include "base/text/smartstring.h"

#include "auto_struct.h"

namespace Refl::Access
{
template <class T>
concept IsSerializable =
    Conv::IsParsable<T> && Conv::IsStringifiable<T>;

template <class Object, bool runtime = false> struct Accessor
{
	std::string (*get)(const Object &);
	void (*set)(Object &, const std::string &);

	template <class... MPs> static consteval Accessor make()
	{
		return {
#ifndef __clang_analyzer__
		    .get =
		        +[](const Object &o)
		        {
			        using Type =
			            Functors::Composite::Res<Object, MPs...>;
			        static_assert(Conv::IsStringifiable<Type>);
			        return std::string{Conv::toString(
			            Functors::FuncPtr<Object, Type, MPs...>(o))};
		        },
		    .set =
		        +[](Object &o, const std::string &str)
		        {
			        using Type =
			            Functors::Composite::Res<Object, MPs...>;
			        static_assert(Conv::IsParsable<Type>);
			        Functors::FuncPtr<Object, Type, MPs...>(o) =
			            Conv::parse<Type>(str);
		        }
#endif
		};
	}
};

template <class Object> struct Accessor<Object, true>
{
	std::function<std::string(const Object &)> get;
	std::function<void(Object &, const std::string &)> set;

	template <class Ptr> static constexpr Accessor make(Ptr ptr)
	{
		return {.get{[ptr](const Object &o)
		            {
			            return std::string{Conv::toString(ptr(o))};
		            }},
		    .set{[ptr](Object &o, const std::string &str)
		        {
			        auto &e = ptr(o);
			        e = Conv::parse<std::remove_cvref_t<decltype(e)>>(
			            str);
		        }}};
	}
};

consteval auto merge_names_size(
    const std::initializer_list<std::string_view> &il)
{
	std::size_t res{};
	for (auto sl : il) res += sl.size() + 1;
	return res ? res - 1 : res;
}

template <std::size_t size>
consteval auto merge_names(
    const std::initializer_list<std::string_view> &il)
{
	std::array<char, size> res{};
	for (auto it = res.data(); auto sl : il) {
		if (it != res.data()) *it++ = '.';
		for (auto ch : sl) *it++ = ch;
	}
	return res;
};

template <class Object, class MPs>
constexpr const void *accessor_pair{};

template <class Object, class... Member>
constexpr std::pair<const std::string_view, Accessor<Object>>
    accessor_pair<Object, std::tuple<Member...>>{
#ifndef __clang_analyzer__
        Name::in_data_name<merge_names<merge_names_size(
            Functors::name_list<Member...>)>(
            Functors::name_list<Member...>)>,
        Accessor<Object>::template make<Member...>()
#endif
    };

template <class T>
using IsSerializableT = std::bool_constant<IsSerializable<T>>;

template <class Object,
    class Members =
#ifndef __clang_analyzer__
        std::remove_cvref_t<
            decltype(all_member_functor_v<Object, IsSerializableT>)>
#else
        std::tuple<>
#endif
    >
constexpr const void *accessor_pairs{};

template <class Object, class... Members>
constexpr std::initializer_list<
    std::pair<const std::string_view, Accessor<Object>>>
    accessor_pairs<Object, std::tuple<Members...>>{
        accessor_pair<Object, Members>...};

template <class Object, bool runtime = false>
const std::map<
    std::conditional_t<runtime, std::string, std::string_view>,
    Accessor<Object, runtime>,
    std::less<>> &
getAccessors()
{
	static_assert(!runtime,
	    "It must implement the runtime function uniquely.");
	static const std::
	    map<std::string_view, Accessor<Object, runtime>, std::less<>>
	        accessors{accessor_pairs<Object>};
	return accessors;
}

template <class Object, bool runtime = false>
const Accessor<Object, runtime> &getAccessor(
    const std::string_view &member)
{
	const auto &accessors = getAccessors<Object, runtime>();
	if (auto it = accessors.find(member); it != accessors.end())
		return it->second;

	static const Accessor<Object, runtime> emptyAccessor{};
	return emptyAccessor;
}

template <class Object, bool runtime = false> auto getAccessorNames()
{
	return std::views::keys(getAccessors<Object, runtime>());
}
}

#endif
