#ifndef VIZZU_REFL_AUTO_ACCESS_H
#define VIZZU_REFL_AUTO_ACCESS_H

#include <map>
#include <string>
#include <string_view>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"

#include "auto_struct.h"

namespace Refl::Access
{
template <class Object> struct Accessor
{
	std::string (*get)(const Object &);
	void (*set)(Object &, const std::string &);
};

#ifndef __clang_analyzer__
template <class Object,
    class Members = decltype(member_functors_v<Object>),
    class = std::make_index_sequence<std::tuple_size_v<Members>>>
constexpr void *accessor_pairs{};

template <class Object, class Members, std::size_t... Ix>
constexpr std::initializer_list<
    std::pair<const std::string_view, Accessor<Object>>>
    accessor_pairs<Object, Members, std::index_sequence<Ix...>> = {
        {std::tuple_element_t<Ix, Members>::name(),
            {.get =
                    +[](const Object &o)
                    {
	                    return Conv::toString(
	                        std::tuple_element_t<Ix, Members>::get(
	                            o));
                    },
                .set =
                    +[](Object &o, const std::string &str)
                    {
	                    using Type = std::remove_cvref_t<
	                        decltype(std::tuple_element_t<Ix,
	                            Members>::get(o))>;
	                    auto &member = Refl::Functors::FuncPtr<Object,
	                        Type,
	                        std::tuple_element_t<Ix, Members>>(o);
	                    member = Conv::parse<Type>(str);
                    }}}...};
#endif

template <class Object>
static const std::map<std::string_view, Accessor<Object>> &
getAccessors()
{
	static const std::map<std::string_view, Accessor<Object>>
	    &accessors{
#ifndef __clang_analyzer__
	        accessor_pairs<Object>
#endif
	    };
	return accessors;
}

template <class Object>
Accessor<Object> getAccessor(const std::string_view &member)
{
	auto &accessors = getAccessors<Object>();
	if (auto it = accessors.find(member); it != accessors.end())
		return it->second;
	return {};
}
}

#endif
