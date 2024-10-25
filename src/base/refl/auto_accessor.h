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
template <class Object, bool runtime = false> struct Accessor
{
	std::string (*get)(const Object &);
	void (*set)(Object &, const std::string &);

	template <template <class> class TransformIn = std::type_identity,
	    class... MPs>
	static consteval Accessor make()
	{
		return {
#ifndef __clang_analyzer__
		    .get =
		        +[](const Object &o)
		        {
			        using Type =
			            Functors::Composite::Res<Object, MPs...>;
			        return Conv::toString(std::as_const(
			            Functors::FuncPtr<Object, Type, MPs...>(o)));
		        },
		    .set =
		        +[](Object &o, const std::string &str)
		        {
			        using Type =
			            Functors::Composite::Res<Object, MPs...>;
			        Functors::FuncPtr<Object, Type, MPs...>(o) =
			            Conv::parse<typename TransformIn<Type>::type>(
			                str);
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
			            return Conv::toString(ptr(o));
		            }},
		    .set{[ptr](Object &o, const std::string &str)
		        {
			        auto &e = ptr(o);
			        e = Conv::parse<std::remove_cvref_t<decltype(e)>>(
			            str);
		        }}};
	}
};

template <class Object,
    class Members =
#ifndef __clang_analyzer__
        decltype(member_functors_v<Object>),
#else
        std::tuple<>,
#endif
    class = std::make_index_sequence<std::tuple_size_v<Members>>>
constexpr const void *accessor_pairs{};

template <class Object, class Members, std::size_t... Ix>
constexpr std::initializer_list<
    std::pair<const std::string_view, Accessor<Object>>>
    accessor_pairs<Object, Members, std::index_sequence<Ix...>>{
        {std::tuple_element_t<Ix, Members>::name(),
            Accessor<Object>::template make<std::type_identity,
                std::tuple_element_t<Ix, Members>>()}...};

template <char sep, const std::initializer_list<std::string_view> &il>
consteval auto merge_names()
{
	std::array<char, Text::SmartString::join<sep>(il).size()> res{};
	std::ranges::copy(Text::SmartString::join<sep>(il), res.data());
	return res;
};

template <template <class> class TransformIn,
    char sep,
    auto Mptr,
    auto... Mptrs>
constexpr inline std::pair<const std::string_view,
    Accessor<decltype(Impl::getBase(Mptr))>>
    mptr_accessor_pair{
#ifndef __clang_analyzer__
        Name::in_data_name<merge_names<sep,
            Functors::name_list<Members::MemberFunctor<Mptr>,
                Members::MemberFunctor<Mptrs>...>>()>,
        Accessor<decltype(Impl::getBase(
            Mptr))>::template make<TransformIn,
            Members::MemberFunctor<Mptr>,
            Members::MemberFunctor<Mptrs>...>()
#endif
    };

template <class Object, bool runtime = false>
const std::map<
    std::conditional_t<runtime, std::string, std::string_view>,
    Accessor<Object, runtime>,
    std::less<>> &
getAccessors()
{
	static_assert(!runtime, "please implement it");
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

	thread_local const Accessor<Object, runtime> emptyAccessor{};
	return emptyAccessor;
}

template <class Object, bool runtime = false> auto getAccessorNames()
{
	return std::ranges::views::keys(getAccessors<Object, runtime>());
}
}

#endif
