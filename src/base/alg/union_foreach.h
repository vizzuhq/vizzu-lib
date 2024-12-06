#ifndef ALG_UNION_FOREACH_H
#define ALG_UNION_FOREACH_H

#include <functional>
#include <ranges>

#include "union_foreach.h"

namespace Alg
{

namespace Impl
{
enum class union_call_t : std::uint8_t {
	both,
	only_left,
	only_right
};

struct single_t
{
} constexpr static single{};
struct multi_t
{
} constexpr static multi{};

template <class F>
concept Property =
    std::same_as<F, single_t> || std::same_as<F, multi_t>;

template <class F,
    class I1,
    class I2 = I1,
    class Ptr1 = decltype(std::to_address(std::declval<I1 &>())),
    class Ptr2 = decltype(std::to_address(std::declval<I2 &>()))>
concept address_binary_invocable =
    std::copy_constructible<F>
    && (std::invocable<F &, Ptr1, Ptr2, union_call_t>
        || std::invocable<F &, Ptr1, Ptr2>);

template <class R>
concept insert_value_returns_iterator = requires {
	{
		std::declval<R &>().insert(
		    std::declval<const std::ranges::range_value_t<R> &>())
	} -> std::same_as<std::ranges::iterator_t<R>>;
};

template <class R>
concept insert_value_returns_pair = requires {
	{
		std::declval<R &>().insert(
		    std::declval<const std::ranges::range_value_t<R> &>())
	} -> std::same_as<std::pair<std::ranges::iterator_t<R>, bool>>;
};

template <std::ranges::forward_range R,
    class CVR = std::remove_cvref_t<R>>
using default_property_t = typename std::conditional_t<
    insert_value_returns_iterator<CVR>,
    std::type_identity<multi_t>,
    std::enable_if<insert_value_returns_pair<CVR>, single_t>>::type;

template <class Fun, class Arg1, class Arg2>
constexpr void
invoke(Fun &&f, Arg1 &&arg1, Arg2 &&arg2, union_call_t val)
{
	if constexpr (std::invocable<Fun &&,
	                  Arg1 &&,
	                  Arg2 &&,
	                  union_call_t>)
		std::invoke(std::forward<Fun>(f),
		    std::forward<Arg1>(arg1),
		    std::forward<Arg2>(arg2),
		    val);
	else
		std::invoke(std::forward<Fun>(f),
		    std::forward<Arg1>(arg1),
		    std::forward<Arg2>(arg2));
}

template <class Exp, class Fun, class Arg>
constexpr const Arg &assignIfSame([[maybe_unused]] Arg &to,
    const Arg &from)
{
	if constexpr (std::same_as<Exp, Fun>)
		return to = from;
	else
		return from;
}

}

using Impl::multi;
using Impl::single;
using Impl::union_call_t;

template <std::forward_iterator It1,
    std::sentinel_for<It1> End1,
    std::forward_iterator It2,
    std::sentinel_for<It2> End2,
    std::indirectly_unary_invocable<It1> Proj1 = std::identity,
    std::indirectly_unary_invocable<It2> Proj2 = std::identity,
    std::indirect_strict_weak_order<std::projected<It1, Proj1>,
        std::projected<It2, Proj2>> Comp = std::ranges::less,
    Impl::address_binary_invocable<It1, It2> Fun,
    Impl::Property Prop>
constexpr void union_foreach(It1 first1,
    End1 last1,
    It2 first2,
    End2 last2,
    Fun f,
    Prop,
    Comp comp = {},
    Proj1 proj1 = {},
    Proj2 proj2 = {})
{
	using enum union_call_t;
	using Impl::assignIfSame;
	using Impl::invoke;
	const std::iter_value_t<It1> *lastValid1{};
	const std::iter_value_t<It2> *lastValid2{};
	while (first1 != last1 || first2 != last2)
		if (first2 == last2
		    || (first1 != last1
		        && std::invoke(comp,
		            std::invoke(proj1, *first1),
		            std::invoke(proj2, *first2))))
			invoke(f,
			    assignIfSame<Impl::single_t, Prop>(lastValid1,
			        std::to_address(first1++)),
			    lastValid2,
			    only_left);
		else if (first1 == last1
		         || std::invoke(comp,
		             std::invoke(proj2, *first2),
		             std::invoke(proj1, *first1)))
			invoke(f,
			    lastValid1,
			    assignIfSame<Impl::single_t, Prop>(lastValid2,
			        std::to_address(first2++)),
			    only_right);
		else {
			auto &&key = std::invoke(proj1, *first1);
			invoke(f,
			    lastValid1 = std::to_address(first1++),
			    lastValid2 = std::to_address(first2++),
			    both);

			if constexpr (std::is_same_v<Impl::multi_t, Prop>) {
				while (first1 != last1 && first2 != last2
				       && !std::invoke(comp,
				           key,
				           std::invoke(proj1, *first1))
				       && !std::invoke(comp,
				           key,
				           std::invoke(proj2, *first2)))
					invoke(f,
					    lastValid1 = std::to_address(first1++),
					    lastValid2 = std::to_address(first2++),
					    both);

				while (first1 != last1
				       && !std::invoke(comp,
				           key,
				           std::invoke(proj1, *first1)))
					invoke(f,
					    std::to_address(first1++),
					    lastValid2,
					    only_left);

				while (first2 != last2
				       && !std::invoke(comp,
				           key,
				           std::invoke(proj2, *first2)))
					invoke(f,
					    lastValid1,
					    std::to_address(first2++),
					    only_right);

				lastValid1 = nullptr;
				lastValid2 = nullptr;
			}
		}
}

template <std::ranges::forward_range R,
    std::indirectly_unary_invocable<std::ranges::iterator_t<R>> Proj =
        std::identity,
    std::indirect_strict_weak_order<
        std::projected<std::ranges::iterator_t<R>, Proj>> Comp =
        std::ranges::less,
    Impl::address_binary_invocable<std::ranges::iterator_t<R>> Fun,
    Impl::Property Prop = Impl::default_property_t<R>>
constexpr void union_foreach(R &&r1,
    R &&r2,
    Fun f,
    Comp comp = {},
    Proj proj = {},
    Prop p = {})
{
	union_foreach(std::ranges::begin(r1),
	    std::ranges::end(r1),
	    std::ranges::begin(r2),
	    std::ranges::end(r2),
	    std::move(f),
	    p,
	    std::move(comp),
	    proj,
	    proj);
}
}

#endif
