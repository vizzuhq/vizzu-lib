#ifndef VIZZU_REFL_AUTO_STRUCT_H
#define VIZZU_REFL_AUTO_STRUCT_H

#include <cstddef>
#include <functional>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

#include "auto_name.h"

namespace Refl
{

namespace Size
{
struct ubiq
{
	std::size_t ignore;

	// NOLINTNEXTLINE
	template <class Type> constexpr operator Type &() const noexcept
	{
		return declval<Type &>();
	}
};

template <class T> struct ubiq_base
{
	std::size_t ignore;
	template <class Type>
	    requires(std::is_base_of_v<std::remove_cvref_t<Type>,
	                 std::remove_cv_t<T>>
	             && !std::is_same_v<std::remove_cvref_t<Type>,
	                 std::remove_cv_t<T>>)
	// NOLINTNEXTLINE
	constexpr operator Type &() const noexcept
	{
		return declval<Type &>();
	}
};

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif

template <class T,
    std::size_t... I,
    class = typename std::enable_if<
        std::is_copy_constructible<T>::value>::type>
consteval typename std::add_pointer<decltype(T{ubiq{I}...})>::type
    enable_if_constructible(std::index_sequence<I...>) noexcept;

#ifdef __clang__
#pragma clang diagnostic pop
#endif

template <class T, std::size_t... B, std::size_t... I>
consteval typename std::add_pointer<decltype(T(ubiq_base<T>{B}...,
    ubiq{I}...))>::type
    enable_if_constructible_base(std::index_sequence<B...>,
        std::index_sequence<I...>) noexcept;

template <template <class, std::size_t, class...> typename,
    class T,
    std::size_t Begin,
    std::size_t Middle>
    requires(Begin == Middle)
consteval std::size_t detect_fields_count(int) noexcept
{
	return Begin;
}

template <template <class, std::size_t, class...> typename,
    class T,
    std::size_t Begin,
    std::size_t Middle>
    requires(Begin < Middle)
consteval std::size_t detect_fields_count(std::size_t) noexcept;

template <template <class, std::size_t, class...> typename EnableIf,
    class T,
    std::size_t Begin,
    std::size_t Middle>
    requires(Begin < Middle && !std::is_void_v<EnableIf<T, Middle>>)
consteval auto detect_fields_count(int) noexcept
{
	return detect_fields_count<EnableIf,
	    T,
	    Middle,
	    Middle + (Middle - Begin + 1) / 2>(0);
}

template <template <class, std::size_t, class...> typename EnableIf,
    class T,
    std::size_t Begin,
    std::size_t Middle>
    requires(Begin < Middle)
consteval std::size_t detect_fields_count(std::size_t) noexcept
{
	return detect_fields_count<EnableIf,
	    T,
	    Begin,
	    Begin + (Middle - Begin) / 2>(0);
}

template <class T,
    std::size_t N,
    class = decltype(enable_if_constructible<T>(
        std::make_index_sequence<N>()))>
using if_constructible_t = std::size_t;

template <std::size_t M> struct FixMax
{
	template <class T,
	    std::size_t N,
	    class = decltype(enable_if_constructible_base<T>(
	        std::make_index_sequence<N>(),
	        std::make_index_sequence<std::min(M - N, M)>()))>
	    requires(N <= M)
	using if_constructible_base_t = std::size_t;
};

template <class T>
consteval std::pair<std::size_t, std::size_t>
aggregate_count() noexcept
{
	using type = std::remove_cvref_t<T>;
	if constexpr (std::is_empty_v<type> || std::is_polymorphic_v<type>
	              || !std::is_aggregate_v<type>
	              || std::is_scalar_v<type>) {
		return {};
	}
	else {
		constexpr auto fields =
		    detect_fields_count<if_constructible_t,
		        T,
		        0,
		        /* sizeof(T) *
		         *   std::numeric_limits<unsigned char>::digits
		         */
		        9>(0);

		constexpr auto bases = detect_fields_count<
		    FixMax<fields>::template if_constructible_base_t,
		    T,
		    0,
		    fields>(0);
		return {bases, fields - bases};
	}
}
}

namespace Loophole
{

#if !defined(__clang__) && defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif

template <class T, std::size_t N> struct tag
{
	friend auto loophole(tag<T, N>);
};

#if !defined(__clang__) && defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

template <class T, class U, std::size_t N, bool B> struct fn_def
{
	friend auto loophole(tag<T, N>)
	{
		return static_cast<std::add_pointer_t<U>>(nullptr);
	}
};
template <class T, class U, std::size_t N>
struct fn_def<T, U, N, true>
{};

template <class T, std::size_t N, bool base = false>
struct loophole_ubiq
{
	template <class U, std::size_t M> static std::size_t ins(...);
	template <class U,
	    std::size_t M,
	    std::size_t = sizeof(loophole(tag<T, M>{}))>
	static char ins(int);

	template <class U,
	    std::size_t = sizeof(
	        fn_def<T, U, N, sizeof(ins<U, N>(0)) == sizeof(char)>)>
	    requires(!base
	             || (std::is_base_of_v<std::remove_cvref_t<U>,
	                     std::remove_cv_t<T>>
	                 && !std::is_same_v<std::remove_cvref_t<U>,
	                     std::remove_cv_t<T>>))
	// NOLINTNEXTLINE
	constexpr operator U &&() const && noexcept
	{
		return declval<U &&>();
	}
};

template <class T,
    class U =
        std::make_index_sequence<Size::aggregate_count<T>().first>,
    class V =
        std::make_index_sequence<Size::aggregate_count<T>().second>>
struct loophole_type_list;

template <typename T, std::size_t... B, std::size_t... Ix>
    requires(requires {
	    T{loophole_ubiq<T, B, true>{}...,
	        loophole_ubiq<T, sizeof...(B) + Ix>{}...};
    })
struct loophole_type_list<T,
    std::index_sequence<B...>,
    std::index_sequence<Ix...>> :
    std::tuple<decltype(T{loophole_ubiq<T, B, true>{}...,
                            loophole_ubiq<T, sizeof...(B) + Ix>{}...},
        0)>
{
	using type = std::tuple<std::tuple<std::remove_reference_t<
	                            decltype(*loophole(tag<T, B>{}))>...>,
	    std::tuple<std::remove_reference_t<decltype(*loophole(
	        tag<T, sizeof...(B) + Ix>{}))>...>>;
};

template <class T>
using aggregate_types_t =
    typename loophole_type_list<std::remove_cvref_t<T>>::type;
}

template <class T>
using bases_t =
    std::tuple_element_t<0, Loophole::aggregate_types_t<T>>;

template <class T>
using members_t =
    std::tuple_element_t<1, Loophole::aggregate_types_t<T>>;

template <std::size_t Ix>
using index_t = std::integral_constant<std::size_t, Ix>;

namespace Impl
{

template <class T,
    class U = std::remove_cv_t<T>,
    class Members = members_t<U>,
    std::size_t = std::tuple_size_v<Members>>
constexpr inline bool same_as_decomposed = false;

template <class T, class U, class Members>
constexpr inline bool same_as_decomposed<T, U, Members, 1> =
    std::is_same_v<std::remove_cv_t<std::tuple_element_t<0, Members>>,
        U>;
}

enum class structure_bindable { no, through_base, through_members };

template <class T,
    class = bases_t<T>,
    bool = Impl::same_as_decomposed<T>,
    bool = !std::is_empty_v<T>,
    size_t = std::tuple_size_v<members_t<T>>>
constexpr inline structure_bindable is_structure_bindable_v =
    structure_bindable::no;

template <class T, size_t members, class... Base>
constexpr inline structure_bindable is_structure_bindable_v<T,
    std::tuple<Base...>,
    false,
    true,
    members> =
    static_cast<structure_bindable>(
        (std::is_empty_v<Base> && ...) * 2);

template <class T, class... Base>
    requires(sizeof...(Base) > 0)
constexpr inline structure_bindable
    is_structure_bindable_v<T, std::tuple<Base...>, false, true, 0> =
        static_cast<structure_bindable>(
            ((2
                 * (1 - std::is_empty_v<Base>)-static_cast<bool>(
                     is_structure_bindable_v<Base>))
                + ... + 0)
            <= 1);

template <class T,
    bool = static_cast<bool>(is_structure_bindable_v<T>),
    class Bases = bases_t<T>>
constexpr inline std::size_t structure_binding_size_v{};

template <class T, class... Base>
constexpr inline std::size_t
    structure_binding_size_v<T, true, std::tuple<Base...>> =
        (structure_binding_size_v<Base>
            + ... + std::tuple_size_v<members_t<T>>);

#ifndef __clang_analyzer__
namespace Members
{
template <class T>
constexpr inline auto get_members(T &&t, index_t<1>) noexcept
{
	auto &[_0] = t;
	return std::forward_as_tuple(_0);
}

template <class T>
constexpr inline auto get_members(T &&t, index_t<2>) noexcept
{
	auto &[_0, _1] = t;
	return std::forward_as_tuple(_0, _1);
}

template <class T>
constexpr inline auto get_members(T &&t, index_t<3>) noexcept
{
	auto &[_0, _1, _2] = t;
	return std::forward_as_tuple(_0, _1, _2);
}

template <class T>
constexpr inline auto get_members(T &&t, index_t<4>) noexcept
{
	auto &[_0, _1, _2, _3] = t;
	return std::forward_as_tuple(_0, _1, _2, _3);
}

template <class T>
constexpr inline auto get_members(T &&t, index_t<5>) noexcept
{
	auto &[_0, _1, _2, _3, _4] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4);
}

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<6>) noexcept
{
	auto &[_0, _1, _2, _3, _4, _5] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4, _5);
}

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<7>) noexcept
{
	auto &[_0, _1, _2, _3, _4, _5, _6] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6);
}

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<8>) noexcept
{
	auto &[_0, _1, _2, _3, _4, _5, _6, _7] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7);
}

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<9>) noexcept
{
	auto &[_0, _1, _2, _3, _4, _5, _6, _7, _8] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7, _8);
}

template <class T, std::size_t I>
constexpr inline auto get_members(T &&, index_t<I>) noexcept
{
	static_assert(I == 0, "Not implemented");
}

template <class T> constexpr inline auto get_members(T &&t) noexcept
{
	return get_members(t, index_t<std::tuple_size_v<members_t<T>>>{});
}

template <class From, class = void>
constexpr inline bool specified_members = false;

template <class From>
constexpr inline bool specified_members<From,
    std::enable_if_t<std::tuple_size_v<decltype(From::members())> != 0
                     && (std::tuple_size_v<members_t<From>> != 0
                         || !std::is_aggregate_v<From>)>> = []
{
	static_assert(!std::is_aggregate_v<From>
	                  || std::tuple_size_v<decltype(From::members())>
	                         == std::tuple_size_v<members_t<From>>,
	    "Not specified all member");
	return true;
}();

template <class From,
    std::size_t Ix,
    bool through_memptrs = specified_members<From>>
struct MemberFunctor;

template <class From, std::size_t Ix>
    requires(!std::is_reference_v<From> && !std::is_const_v<From>)
struct MemberFunctor<From, Ix, true>
{
	template <class F = From>
	constexpr static inline decltype(auto) get(const F &t)
	{
		constexpr auto G = std::get<Ix>(F::members());
		return std::invoke(G, t);
	}

	template <class F = From>
	using type =
	    std::remove_cvref_t<decltype(get(std::declval<F>()))>;

	template <class F = From>
	consteval static inline std::string_view name()
	{
		return Refl::Name::in_data_name<
		    Refl::Name::name<decltype(std::get<Ix>(F::members())),
		        std::get<Ix>(F::members())>()>;
	}
};

template <class From, std::size_t Ix>
    requires(!std::is_reference_v<From> && !std::is_const_v<From>
             && is_structure_bindable_v<From>
                    == structure_bindable::through_members)
struct MemberFunctor<From, Ix, false>
{
	constexpr static inline decltype(auto) get(const From &t)
	{
		return std::get<Ix>(get_members(t));
	}

	template <class = void>
	using type =
	    std::remove_cvref_t<decltype(get(std::declval<From>()))>;

	template <class = void>
	consteval static inline std::string_view name()
	{
		return Refl::Name::in_data_name<Refl::Name::name<void,
		    Name::Wrapper{
		        &std::get<Ix>(get_members(declval<From &>()))}>()>;
	}
};

template <class T, std::size_t... Ix>
consteval auto get_members_by_bind(std::index_sequence<Ix...>)
{
	return std::tuple<MemberFunctor<T, Ix>...>{};
}

template <class T, std::size_t... Ix>
consteval auto get_members_by_memptrs(std::index_sequence<Ix...>)
{
	if constexpr ((std::is_same_v<
	                   std::remove_cvref_t<decltype(std::get<Ix>(
	                       T::members()))>,
	                   std::remove_cvref_t<decltype(std::ignore)>>
	                  || ...)) {

		using X = decltype(std::tuple_cat(
		    std::declval<std::conditional_t<
		        std::is_same_v<std::remove_cvref_t<decltype(std::get<
		                           Ix>(T::members()))>,
		            std::remove_cvref_t<decltype(std::ignore)>>,
		        std::tuple<>,
		        std::tuple<index_t<Ix>>>>()...));

		constexpr auto sequence = std::apply(
		    []<std::size_t... Ixs>(index_t<Ixs>...)
		    {
			    return std::index_sequence<Ixs...>{};
		    },
		    X{});
		return get_members_by_memptrs<T>(sequence);
	}
	else {
		return std::tuple<MemberFunctor<T, Ix>...>{};
	}
}

template <class T>
    requires(std::tuple_size_v<decltype(T::members())> != 0
             && (std::tuple_size_v<members_t<T>> != 0
                 || !std::is_aggregate_v<T>))
consteval auto get_member_functors(std::nullptr_t)
{
	static_assert(!std::is_aggregate_v<T>
	                  || std::tuple_size_v<decltype(T::members())>
	                         == std::tuple_size_v<members_t<T>>,
	    "Not specified all member");
	return get_members_by_memptrs<T>(std::make_index_sequence<
	    std::tuple_size_v<decltype(T::members())>>{});
}

template <class T>
    requires(is_structure_bindable_v<T>
             == structure_bindable::through_members)
consteval auto get_member_functors(void *)
{
	return get_members_by_bind<T>(
	    std::make_index_sequence<structure_binding_size_v<T>>{});
}

template <class T>
    requires(is_structure_bindable_v<T>
                 != structure_bindable::through_members
             && std::tuple_size_v<members_t<T>> == 0
             && 0 < std::tuple_size_v<bases_t<T>>)
consteval auto get_member_functors(void *)
{
	return std::tuple{};
}

template <class T>
    requires(is_structure_bindable_v<T> == structure_bindable::no
             && std::tuple_size_v<members_t<T>> == 0
             && std::tuple_size_v<bases_t<T>> == 0
             && !std::is_empty_v<T>)
consteval auto get_member_functors(void *)
{
	static_assert(!std::is_polymorphic_v<T>);
	static_assert(std::is_aggregate_v<T>);
	return std::tuple{};
}
}

namespace Functors
{
namespace Composite
{
template <class T, class U>
constexpr inline auto operator>>(U &&arg, const T &)
    -> decltype(T::get(std::forward<U &&>(arg)))
{
	return T::get(std::forward<U &&>(arg));
}
}

template <class T, class U, class... MemberFunctors>
constexpr static inline auto FuncPtr = +[](const T &t) -> U &
{
	using Composite::operator>>;
	auto &v = (t >> ... >> MemberFunctors{});
	return const_cast<std::remove_cvref_t<decltype(v)> &>( // NOLINT
	    v);
};

template <class... MemberFunctors>
constexpr static inline std::initializer_list<std::string_view>
    name_list{MemberFunctors::name()...};

template <class T, class Visitor> struct Applier
{
	template <class U, class... MemberFunctors>
	    requires(!std::is_same_v<T, U>
	             && std::is_invocable_v<Visitor &,
	                 U &(*)(const T &),
	                 const std::initializer_list<std::string_view> &>)
	constexpr static inline __attribute__((always_inline)) void apply(
	    Visitor &v) noexcept
	{
		v(FuncPtr<T, U, MemberFunctors...>,
		    name_list<MemberFunctors...>);
	}

	template <class U, class... MemberFunctors>
	    requires(!std::is_same_v<T, U>
	             && std::is_invocable_v<Visitor &, U &(*)(const T &)>
	             && !std::is_invocable_v<Visitor &,
	                 U &(*)(const T &),
	                 const std::initializer_list<std::string_view> &>)
	constexpr static inline __attribute__((always_inline)) void apply(
	    Visitor &v) noexcept
	{
		v(FuncPtr<T, U, MemberFunctors...>);
	}

	template <class U, class... MemberFunctors>
	constexpr static inline __attribute__((always_inline)) void apply(
	    [[maybe_unused]] Visitor &v) noexcept
	{
		if constexpr (!std::is_empty_v<U>) {
			if constexpr (std::tuple_size_v<bases_t<U>> > 0) {
				std::invoke(
				    [&v]<class... Bases>(std::tuple<Bases...> *)
				    {
					    (Functors::Applier<T,
					         Visitor>::template apply<Bases,
					         MemberFunctors...>(v),
					        ...);
				    },
				    std::add_pointer_t<bases_t<U>>{});
			}

			if constexpr (constexpr auto members =
			                  Members::get_member_functors<U>(
			                      nullptr);
			              std::tuple_size_v<decltype(members)> > 0) {
				std::apply(
				    [&v]<class... MF>(MF...)
				    {
					    (Functors::Applier<T, Visitor>::
					            template apply<
					                typename MF::template type<>,
					                MemberFunctors...,
					                MF>(v),
					        ...);
				    },
				    members);
			}
		}
	}
};

template <class Visitor,
    class Tuple,
    class = std::make_index_sequence<std::tuple_size_v<Tuple>>>
struct GetterVisitor;

template <class Visitor, class... Ts, std::size_t... Ix>
struct GetterVisitor<Visitor,
    std::tuple<Ts...>,
    std::index_sequence<Ix...>>
{
	Visitor &visitor;
	std::tuple<Ts...> ts;

	constexpr inline GetterVisitor(Visitor &p_visitor,
	    std::tuple<Ts...> t) :
	    visitor(p_visitor),
	    ts(std::move(t))
	{}

	template <class Getter>
	constexpr inline std::invoke_result_t<Visitor &,
	    std::invoke_result_t<Getter, Ts>...>
	operator()(Getter &&getter) const noexcept
	{
		return std::invoke(visitor,
		    std::invoke(getter, std::get<Ix>(ts))...);
	}

	template <class Getter>
	constexpr inline std::invoke_result_t<Visitor &,
	    std::invoke_result_t<Getter, Ts>...,
	    const std::initializer_list<std::string_view> &>
	operator()(Getter &&getter,
	    const std::initializer_list<std::string_view> &sv = {})
	    const noexcept
	{
		return std::invoke(visitor,
		    std::invoke(getter, std::get<Ix>(ts))...,
		    sv);
	}
};

}
#endif

template <class T, class Visitor>
constexpr inline __attribute__((always_inline)) auto visit(
    [[maybe_unused]] Visitor &&visitor)
{
#ifndef __clang_analyzer__
	return Functors::Applier<T, Visitor>::template apply<T>(visitor);
#endif
}

template <class Visitor, class T, class... Ts>
    requires((std::is_same_v<std::remove_cvref_t<T>,
                  std::remove_cvref_t<Ts>>
              && ...))
constexpr inline void visit([[maybe_unused]] Visitor &&visitor,
    [[maybe_unused]] T &visitable,
    [[maybe_unused]] Ts &&...ts)
{
#ifndef __clang_analyzer__
	using TT = std::remove_cvref_t<T>;
	visit<TT>(
	    Functors::GetterVisitor<Visitor, std::tuple<T &, Ts &...>>{
	        visitor,
	        {visitable, ts...}});
#endif
}

}

#endif