#ifndef VIZZU_REFL_AUTO_STRUCT_H
#define VIZZU_REFL_AUTO_STRUCT_H

#include <cstddef>
#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>

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

template <class T, std::size_t... I>
consteval std::add_pointer_t<decltype(T{ubiq{I}...})>
    enable_if_constructible(std::index_sequence<I...>) noexcept;

#ifdef __clang__
#pragma clang diagnostic pop
#endif

template <class T, std::size_t... B, std::size_t... I>
consteval std::add_pointer_t<decltype(T(ubiq_base<T>{B}...,
    ubiq{I}...))>
    enable_if_constructible_base(std::index_sequence<B...>,
        std::index_sequence<I...>) noexcept;

template <template <class, std::size_t, class...> typename,
    class,
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
    requires(Begin < Middle && EnableIf<T, Middle>::value)
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

template <class T, std::size_t N>
using if_constructible_t = std::bool_constant<requires {
	enable_if_constructible<T>(std::make_index_sequence<N>());
}>;

template <std::size_t M> struct FixMax
{
	template <class T, std::size_t N>
	using if_constructible_base_t =
	    std::bool_constant<N <= M &&requires {
		    enable_if_constructible_base<T>(
		        std::make_index_sequence<N>(),
		        std::make_index_sequence<std::min(M - N, M)>());
	    }>;
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
		        15>(0);

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

enum class structure_bindable : std::uint8_t {
	no,
	through_bases,
	through_members
};

template <class T,
    bool = !Impl::same_as_decomposed<T> && !std::is_empty_v<T>,
    class = bases_t<T>,
    size_t = std::tuple_size_v<members_t<T>>>
constexpr inline structure_bindable is_structure_bindable_v =
    structure_bindable::no;

template <class T, size_t members, class... Base>
constexpr inline structure_bindable
    is_structure_bindable_v<T, true, std::tuple<Base...>, members> =
        static_cast<structure_bindable>(
            (std::is_empty_v<Base> && ...) * 2);

template <class T, class... Base>
    requires(sizeof...(Base) > 0)
constexpr inline structure_bindable
    is_structure_bindable_v<T, true, std::tuple<Base...>, 0> =
        static_cast<structure_bindable>(
            ((std::is_empty_v<Base>
                 || static_cast<bool>(is_structure_bindable_v<Base>))
                && ...));

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

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<10>) noexcept
{
	auto &[_0, _1, _2, _3, _4, _5, _6, _7, _8, _9] = t;
	return std::forward_as_tuple(_0,
	    _1,
	    _2,
	    _3,
	    _4,
	    _5,
	    _6,
	    _7,
	    _8,
	    _9);
}

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<11>) noexcept
{
	auto &[_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10] = t;
	return std::forward_as_tuple(_0,
	    _1,
	    _2,
	    _3,
	    _4,
	    _5,
	    _6,
	    _7,
	    _8,
	    _9,
	    _10);
}

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<12>) noexcept
{
	auto &[_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11] = t;
	return std::forward_as_tuple(_0,
	    _1,
	    _2,
	    _3,
	    _4,
	    _5,
	    _6,
	    _7,
	    _8,
	    _9,
	    _10,
	    _11);
}

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<13>) noexcept
{
	auto &[_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12] = t;
	return std::forward_as_tuple(_0,
	    _1,
	    _2,
	    _3,
	    _4,
	    _5,
	    _6,
	    _7,
	    _8,
	    _9,
	    _10,
	    _11,
	    _12);
}

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<14>) noexcept
{
	auto &[_0,
	    _1,
	    _2,
	    _3,
	    _4,
	    _5,
	    _6,
	    _7,
	    _8,
	    _9,
	    _10,
	    _11,
	    _12,
	    _13] = t;
	return std::forward_as_tuple(_0,
	    _1,
	    _2,
	    _3,
	    _4,
	    _5,
	    _6,
	    _7,
	    _8,
	    _9,
	    _10,
	    _11,
	    _12,
	    _13);
}

template <class T>
// NOLINTNEXTLINE
constexpr inline auto get_members(T &&t, index_t<15>) noexcept
{
	auto &[_0,
	    _1,
	    _2,
	    _3,
	    _4,
	    _5,
	    _6,
	    _7,
	    _8,
	    _9,
	    _10,
	    _11,
	    _12,
	    _13,
	    _14] = t;
	return std::forward_as_tuple(_0,
	    _1,
	    _2,
	    _3,
	    _4,
	    _5,
	    _6,
	    _7,
	    _8,
	    _9,
	    _10,
	    _11,
	    _12,
	    _13,
	    _14);
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

template <auto Ix,
    class From = decltype(Impl::getBase(Ix)),
    bool through_memptrs = specified_members<From>>
struct MemberFunctor;

template <class From, std::size_t Ix>
    requires(!std::is_reference_v<From> && !std::is_const_v<From>)
struct MemberFunctor<Ix, From, true> :
    MemberFunctor<std::get<Ix>(From::members()), From, true>
{};

template <auto G, class From, bool through_memptrs>
    requires std::is_member_object_pointer_v<decltype(G)>
struct MemberFunctor<G, From, through_memptrs>
{
	constexpr static inline decltype(auto) get(const From &t)
	{
		return std::invoke(G, t);
	}

	template <class F = From>
	using type = std::remove_cvref_t<decltype(std::invoke(G,
	    std::declval<F>()))>;

	template <class F = From>
	consteval static inline std::string_view name()
	{
		return Name::in_data_name<Name::name<type<F> F::*, G>()>;
	}
};

template <std::size_t Ix, class From>
    requires(!std::is_reference_v<From> && !std::is_const_v<From>
             && is_structure_bindable_v<From>
                    == structure_bindable::through_members)
struct MemberFunctor<Ix, From, false>
{
	constexpr static inline decltype(auto) get(const From &t)
	{
		return std::get<Ix>(get_members(t));
	}

	template <class F = From>
	using type =
	    std::remove_cvref_t<decltype(get(std::declval<F>()))>;

	template <class F = From>
	consteval static inline std::string_view name()
	{
		return Name::in_data_name<Name::name<void,
		    Name::Wrapper{
		        &std::get<Ix>(get_members(declval<F &>()))}>()>;
	}
};

template <class T, std::size_t... Ix>
consteval auto get_members_by_bind(std::index_sequence<Ix...>)
{
	return std::tuple<MemberFunctor<Ix, T>...>{};
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
		return std::tuple<MemberFunctor<Ix, T>...>{};
	}
}

template <class T>
    requires(specified_members<T>)
consteval auto get_member_functors(std::nullptr_t)
{
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
             && std::tuple_size_v<bases_t<T>> == 0)
consteval auto get_member_functors(void *)
{
	static_assert(!std::is_polymorphic_v<T>);
	static_assert(std::is_aggregate_v<T>);
	return std::tuple{};
}
}

template <class T>
constexpr static inline auto member_functors_v =
    Members::get_member_functors<T>(nullptr);

template <class MF>
using member_functor_t = typename MF::template type<>;

template <class T>
concept is_reflectable =
    std::is_empty_v<T> || Members::specified_members<T>
    || (std::is_aggregate_v<T>
        && is_structure_bindable_v<T> != structure_bindable::no);

template <class T, template <class> class Checker, class... PreMPs>
constexpr static auto all_member_functor_v{
    []<class... Bases, class... MF>(std::tuple<Bases...> *,
        std::tuple<MF...>)
    {
	    if constexpr (Checker<T>::value) {
		    return std::tuple<std::tuple<PreMPs...>>{};
	    }
	    else if constexpr (is_reflectable<T>) {
		    return decltype(std::tuple_cat(
		        all_member_functor_v<Bases, Checker, PreMPs...>...,
		        all_member_functor_v<member_functor_t<MF>,
		            Checker,
		            PreMPs...,
		            MF>...)){};
	    }
    }(std::add_pointer_t<bases_t<T>>{}, member_functors_v<T>)};

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

template <class T, class... MemberFunctors>
using Res = std::remove_cvref_t<decltype((
    std::declval<const T &>() >> ... >> MemberFunctors{}))>;
}

template <class T, class U, class... MemberFunctors>
constexpr static inline auto FuncPtr = +[](const T &t) -> U &
{
	using Composite::operator>>;
	const auto *v = &(t >> ... >> MemberFunctors{});
	return const_cast<std::remove_cvref_t<decltype(*v)> &>( // NOLINT
	    *v);
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
	constexpr static __attribute__((always_inline)) auto apply(
	    Visitor &v) noexcept
	{
		return v(FuncPtr<T, U, MemberFunctors...>,
		    name_list<MemberFunctors...>);
	}

	template <class U, class... MemberFunctors>
	    requires(!std::is_same_v<T, U>
	             && std::is_invocable_v<Visitor &, U &(*)(const T &)>
	             && !std::is_invocable_v<Visitor &,
	                 U &(*)(const T &),
	                 const std::initializer_list<std::string_view> &>)
	constexpr static __attribute__((always_inline)) auto apply(
	    Visitor &v) noexcept
	{
		return v(FuncPtr<T, U, MemberFunctors...>);
	}

	template <class U, class... MemberFunctors>
	constexpr static __attribute__((always_inline)) auto apply(
	    [[maybe_unused]] Visitor &v) noexcept
	{
		static_assert(is_reflectable<U>, "Unable to run reflection");

		return std::invoke(
		    [&v]<class... Bases, class... MF>(std::tuple<Bases...> *,
		        std::tuple<MF...>)
		    {
			    if constexpr (
			        std::is_same_v<
			            std::tuple<Bases..., member_functor_t<MF>...>,
			            std::tuple<decltype(apply<Bases,
			                           MemberFunctors...>(v))...,
			                decltype(apply<member_functor_t<MF>,
			                    MemberFunctors...,
			                    MF>(v))...>>) {
				    return U{apply<Bases, MemberFunctors...>(v)...,
				        apply<member_functor_t<MF>,
				            MemberFunctors...,
				            MF>(v)...};
			    }
			    else {
				    (apply<Bases, MemberFunctors...>(v), ...);
				    (apply<member_functor_t<MF>,
				         MemberFunctors...,
				         MF>(v),
				        ...);
			    }
		    },
		    std::add_pointer_t<bases_t<U>>{},
		    member_functors_v<U>);
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
	Visitor &&visitor;
	std::tuple<Ts...> ts;

	constexpr GetterVisitor(Visitor &&p_visitor,
	    std::tuple<Ts...> t) :
	    visitor(std::forward<Visitor>(p_visitor)),
	    ts(std::move(t))
	{}

	template <class Getter>
	constexpr std::invoke_result_t<Visitor &,
	    std::invoke_result_t<Getter, Ts>...>
	operator()(Getter &&getter) const noexcept
	{
		return std::invoke(visitor,
		    std::invoke(getter, std::get<Ix>(ts))...);
	}

	template <class Getter>
	constexpr std::invoke_result_t<Visitor &,
	    std::invoke_result_t<Getter, Ts>...,
	    const std::initializer_list<std::string_view> &>
	operator()(Getter &&getter,
	    const std::initializer_list<std::string_view> &sv)
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
constexpr __attribute__((always_inline)) auto visit(Visitor &&visitor)
{
#ifndef __clang_analyzer__
	return Functors::Applier<T, Visitor>::template apply<T>(visitor);
#else
	std::ignore = std::forward<Visitor>(visitor);
#endif
}

template <class Res = void, class Visitor, class T, class... Ts>
    requires((std::is_same_v<std::remove_cvref_t<T>,
                  std::remove_cvref_t<Ts>>
              && ...))
constexpr Res
visit(Visitor &&visitor, [[maybe_unused]] T &visitable, Ts &&...ts)
{
#ifndef __clang_analyzer__
	using TT = std::remove_cvref_t<T>;
	using GVisitor =
	    Functors::GetterVisitor<Visitor, std::tuple<T &, Ts &&...>>;
	GVisitor getter_visitor{std::forward<Visitor>(visitor),
	    {visitable, std::forward<Ts>(ts)...}};
	return Functors::Applier<TT, GVisitor>::template apply<TT>(
	    getter_visitor);
#else
	std::ignore = std::forward<Visitor>(visitor);
	std::ignore =
	    std::forward_as_tuple<Ts...>(std::forward<Ts>(ts)...);
	if constexpr (!std::is_void_v<Res>) return {};
#endif
}

}

#endif