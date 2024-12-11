#ifndef ALG_MERGE_H
#define ALG_MERGE_H

#include <algorithm>
#include <cstdint>
#include <functional>
#include <ranges>

namespace Alg
{

// closest to std::ranges::merge, but:
// - added new projection parameter to projection both ranges
// - comparison is a three-way comparison which requires weak_order
//   on projected values
// - added new transformer_1 and transformer_2 parameters to transform
//   the values at left and right side if no merging happens
// - added need_merge parameter to decide if merging is necessary
//   of 2 equivalent values or just transforming all values
// - added merger parameter to merge the values if merging happens
// - output argument can be range which used as inserter_iterator
// - arguments are passed as struct to avoid argument swapping
//   and used as named arguments

namespace Merge
{
struct get_first
{
	template <class T, class... Args>
	constexpr auto operator()(T &&t, const Args &...) const noexcept
	{
		return std::forward<T>(t);
	}
};

struct always
{
	template <class... Ts>
	constexpr auto operator()(const Ts &...) const noexcept
	{
		return res;
	}
	bool res{true};
};

template <typename T>
concept comparison_category =
    !std::is_void_v<std::common_comparison_category_t<T>>;

template <typename T, typename Cat>
concept compares_as =
    comparison_category<Cat>
    && std::same_as<std::common_comparison_category_t<T, Cat>, Cat>;

template <typename R,
    typename T,
    typename U = T,
    typename Cat = std::weak_ordering>
concept three_way_order =
    std::regular_invocable<R, T, U>
    && compares_as<std::invoke_result_t<R, T, U>, Cat>;

template <typename R,
    typename Iter1,
    typename Iter2 = Iter1,
    typename Cat = std::weak_ordering>
concept indirect_three_way_order =
    std::indirectly_readable<Iter1> && std::indirectly_readable<Iter2>
    && std::copy_constructible<R>
    && three_way_order<R &,
        std::iter_value_t<Iter1> &,
        std::iter_value_t<Iter2> &,
        Cat>
    && three_way_order<R &,
        std::iter_value_t<Iter1> &,
        std::iter_reference_t<Iter2>,
        Cat>
    && three_way_order<R &,
        std::iter_reference_t<Iter1>,
        std::iter_value_t<Iter2> &,
        Cat>
    && three_way_order<R &,
        std::iter_reference_t<Iter1>,
        std::iter_reference_t<Iter2>,
        Cat>
    && three_way_order<R &,
        std::iter_common_reference_t<Iter1>,
        std::iter_common_reference_t<Iter2>,
        Cat>;

template <typename... Ts>
concept common_references =
    (std::common_reference_with<std::common_reference_t<Ts...>, Ts>
        && ...);

template <typename R, typename Iter1, typename Iter2 = Iter1>
concept indirectly_binary_invocable =
    std::indirectly_readable<Iter1> && std::indirectly_readable<Iter2>
    && std::copy_constructible<R>
    && std::invocable<R &,
        std::iter_value_t<Iter1> &,
        std::iter_value_t<Iter2> &>
    && std::invocable<R &,
        std::iter_value_t<Iter1> &,
        std::iter_reference_t<Iter2>>
    && std::invocable<R &,
        std::iter_reference_t<Iter1>,
        std::iter_value_t<Iter2> &>
    && std::invocable<R &,
        std::iter_reference_t<Iter1>,
        std::iter_reference_t<Iter2>>
    && std::invocable<R &,
        std::iter_common_reference_t<Iter1>,
        std::iter_common_reference_t<Iter2>>
    && common_references<std::invoke_result_t<R &,
                             std::iter_value_t<Iter1> &,
                             std::iter_value_t<Iter2> &>,
        std::invoke_result_t<R &,
            std::iter_value_t<Iter1> &,
            std::iter_reference_t<Iter2>>,
        std::invoke_result_t<R &,
            std::iter_reference_t<Iter1>,
            std::iter_value_t<Iter2> &>,
        std::invoke_result_t<R &,
            std::iter_reference_t<Iter1>,
            std::iter_reference_t<Iter2>>>;

template <typename R, typename... Its>
concept indirectly_unary_invocable_all =
    (std::indirectly_unary_invocable<R, Its> && ...);

template <class U, class T = std::remove_reference_t<U>>
concept insertable_range_or_iterator =
    std::weakly_incrementable<std::remove_cv_t<T>>
    || (std::ranges::range<U>
        && std::weakly_incrementable<std::insert_iterator<T>>
        && requires(U &cont,
            std::ranges::iterator_t<U> i,
            const std::ranges::range_value_t<U> &v) {
	           std::inserter(cont, std::ranges::end(cont));
	           i = cont.insert(i, v);
	           ++i;
           });

template <class T>
struct borrowed_inserter_or_self :
    std::conditional<std::ranges::borrowed_range<T>,
        std::insert_iterator<std::remove_reference_t<T>>,
        std::ranges::dangling>
{};

template <class T>
    requires(std::weakly_incrementable<std::remove_cvref_t<T>>)
struct borrowed_inserter_or_self<T> :
    std::type_identity<std::remove_cvref_t<T>>
{};

template <class T>
using borrowed_inserter_or_self_t =
    typename borrowed_inserter_or_self<T>::type;

template <class Proj1 = std::identity,
    class Proj2 = std::identity,
    class Proj = std::identity,
    class Comp = decltype(std::weak_order),
    class Transform1 = std::identity,
    class Transform2 = std::identity,
    class NeedMerge = always,
    class Merge = get_first>
struct args
{
	[[no_unique_address]] Proj1 projection_1{};
	[[no_unique_address]] Proj2 projection_2{};
	[[no_unique_address]] Proj projection{};
	[[no_unique_address]] Comp comparator{};
	[[no_unique_address]] Transform1 transformer_1{};
	[[no_unique_address]] Transform2 transformer_2{};
	[[no_unique_address]] NeedMerge need_merge{};
	[[no_unique_address]] Merge merger{};
};

template <std::input_iterator It1,
    std::sentinel_for<It1> End1,
    std::input_iterator It2,
    std::sentinel_for<It2> End2,
    std::weakly_incrementable Out,
    std::indirectly_unary_invocable<It1> Proj1,
    std::indirectly_unary_invocable<It2> Proj2,
    indirectly_unary_invocable_all<std::projected<It1, Proj1>,
        std::projected<It2, Proj2>> Proj,
    indirect_three_way_order<
        std::projected<std::projected<It1, Proj1>, Proj>,
        std::projected<std::projected<It2, Proj2>, Proj>> Comp,
    std::indirectly_unary_invocable<It1> Transform1,
    std::indirectly_unary_invocable<It2> Transform2,
    std::indirect_binary_predicate<It1, It2> NeedMerge,
    indirectly_binary_invocable<It1, It2> Merge>
constexpr std::ranges::in_in_out_result<It1, It2, Out> merge(
    It1 first1,
    End1 last1,
    It2 first2,
    End2 last2,
    Out result,
    args<Proj1,
        Proj2,
        Proj,
        Comp,
        Transform1,
        Transform2,
        NeedMerge,
        Merge> &&args = {})
{
	while (first1 != last1 && first2 != last2)
		if (auto cmp = std::invoke(args.comparator,
		        std::invoke(args.projection,
		            std::invoke(args.projection_1, *first1)),
		        std::invoke(args.projection,
		            std::invoke(args.projection_2, *first2)));
		    is_eq(cmp)
		    && std::invoke(args.need_merge, *first1, *first2)) {
			*result = std::invoke(args.merger, *first1, *first2);
			++first1;
			++first2;
			++result;
		}
		else {
			if (is_lteq(cmp)) {
				*result = std::invoke(args.transformer_1, *first1);
				++first1;
				++result;
			}
			if (is_gteq(cmp)) {
				*result = std::invoke(args.transformer_2, *first2);
				++first2;
				++result;
			}
		}
	auto copy1 = std::ranges::transform(std::move(first1),
	    std::move(last1),
	    std::move(result),
	    args.transformer_1);
	auto copy2 = std::ranges::transform(std::move(first2),
	    std::move(last2),
	    std::move(copy1.out),
	    args.transformer_2);
	return {std::move(copy1.in),
	    std::move(copy2.in),
	    std::move(copy2.out)};
}

template <std::ranges::input_range R1,
    std::ranges::input_range R2,
    insertable_range_or_iterator Out,
    class... Args>
constexpr std::ranges::in_in_out_result<
    std::ranges::borrowed_iterator_t<R1>,
    std::ranges::borrowed_iterator_t<R2>,
    borrowed_inserter_or_self_t<Out>>
merge(R1 &&range1,
    R2 &&range2,
    Out &&result,
    args<Args...> &&args = {})
{
	if constexpr (std::weakly_incrementable<
	                  std::remove_cvref_t<Out>>) {
		return ::Alg::Merge::merge(std::ranges::begin(range1),
		    std::ranges::end(range1),
		    std::ranges::begin(range2),
		    std::ranges::end(range2),
		    result,
		    std::move(args));
	}
	else {
		return ::Alg::Merge::merge(std::ranges::begin(range1),
		    std::ranges::end(range1),
		    std::ranges::begin(range2),
		    std::ranges::end(range2),
		    std::inserter(result, std::ranges::end(result)),
		    std::move(args));
	}
}
}

using Merge::merge;
template <class... Args> using merge_args = Merge::args<Args...>;

}

#endif
