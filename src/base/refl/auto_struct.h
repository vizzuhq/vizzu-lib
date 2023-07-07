#ifndef VIZZU_REFL_AUTO_STRUCT_H
#define VIZZU_REFL_AUTO_STRUCT_H

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include <limits>
#include <functional>

#include "auto_name.h"

namespace Refl
{

template <class... Ts> struct tuple
{
	template <std::size_t I>
	using type = std::tuple_element_t<I, std::tuple<Ts...>>;

	constexpr static inline auto count = sizeof...(Ts);
};

template<class T>
extern T not_exists;

template<class T>
constexpr T declval() {
	return std::forward<T>(not_exists<T>);
}

namespace Size
{
struct ubiq
{
	std::size_t ignore;
	template <class Type> constexpr operator Type &() const noexcept
	{
		return declval<Type &>();
	}
};

template <class T> struct ubiq_base
{
	std::size_t ignore;
	template <class Type,
	    std::enable_if_t<std::is_base_of_v<std::remove_cvref_t<Type>,
	                         std::remove_cv_t<T>>
	                     && !std::is_same_v<std::remove_cvref_t<Type>,
	                         std::remove_cv_t<T>>> * = nullptr>
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
consteval auto enable_if_constructible(
    std::index_sequence<I...>) noexcept ->
    typename std::add_pointer<decltype(T{ubiq{I}...})>::type;

#ifdef __clang__
#pragma clang diagnostic pop
#endif

template <class T, std::size_t... B, std::size_t... I>
consteval auto enable_if_constructible_base(std::index_sequence<B...>,
    std::index_sequence<I...>) noexcept ->
    typename std::add_pointer<decltype(T(ubiq_base<T>{B}...,
        ubiq{I}...))>::type;

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
consteval std::size_t detect_fields_count(long) noexcept;

template <template <class, std::size_t, class...> typename EnableIf,
    class T,
    std::size_t Begin,
    std::size_t Middle>
    requires(Begin < Middle)
consteval auto detect_fields_count(int) noexcept
    -> EnableIf<T, Middle>
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
consteval std::size_t detect_fields_count(long) noexcept
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
		        sizeof(T)
		            * std::numeric_limits<unsigned char>::digits>(0);

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
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif

template <class T, std::size_t N> struct tag
{
	friend auto loophole(tag<T, N>);
};

#if !defined(__clang__) && defined(__GNUC__)
#   pragma GCC diagnostic pop
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
	    std::enable_if_t<
	        !base
	        || (std::is_base_of_v<std::remove_cvref_t<U>,
	                std::remove_cv_t<T>>
	            && !std::is_same_v<std::remove_cvref_t<U>,
	                std::remove_cv_t<T>>)> * = nullptr,
	    std::size_t = sizeof(
	        fn_def<T, U, N, sizeof(ins<U, N>(0)) == sizeof(char)>)>
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

template <typename T, std::size_t... B, std::size_t... I>
struct loophole_type_list<T,
    std::index_sequence<B...>,
    std::index_sequence<I...>> :
    Refl::tuple<decltype(T{loophole_ubiq<T, B, true>{}...,
                             loophole_ubiq<T, sizeof...(B) + I>{}...},
        0)>
{
	using type =
	    Refl::tuple<Refl::tuple<std::remove_reference_t<
	                    decltype(*loophole(tag<T, B>{}))>...>,
	        Refl::tuple<std::remove_reference_t<decltype(*loophole(
	            tag<T, sizeof...(B) + I>{}))>...>>;
};

template <class T>
using aggregate_types_t =
    typename Loophole::loophole_type_list<T>::type;
}

template <class T>
using bases_t =
    typename Loophole::aggregate_types_t<T>::template type<0>;

template <class T>
using members_t =
    typename Loophole::aggregate_types_t<T>::template type<1>;

template <class T, std::size_t = members_t<T>::count>
constexpr inline bool not_same_as_decomposed = true;

template <class T>
constexpr inline bool not_same_as_decomposed<T, 1> = !std::is_same_v<
    std::remove_cv_t<typename members_t<T>::template type<0>>,
    std::remove_cv_t<T>>;

template <class T, class Bases = bases_t<T>>
constexpr inline bool is_structure_bindable_v = false;

template <class T, class... Base>
constexpr inline bool is_structure_bindable_v<T, tuple<Base...>> =
    (not_same_as_decomposed<T>
        && Loophole::aggregate_types_t<T>::count > 0)
    && (((is_structure_bindable_v<Base> + ... + 0) == 1
            && members_t<T>::count == 0)
        || (std::is_empty_v<Base> && ... && !std::is_empty_v<T>));

template <class T,
    bool = is_structure_bindable_v<T>,
    class Bases = bases_t<T>>
constexpr inline std::size_t structure_binding_size_v{};

template <class T, class... Base>
constexpr inline std::size_t
    structure_binding_size_v<T, true, tuple<Base...>> =
        (structure_binding_size_v<Base> + ... + members_t<T>::count);

namespace Members
{
template <std::size_t>
constexpr inline auto get_members = [](auto &)
{
};

template <>
constexpr inline auto get_members<1> = [](auto &t)
{
	auto &[_0] = t;
	return std::forward_as_tuple(_0);
};

template <>
constexpr inline auto get_members<2> = [](auto &t)
{
	auto &[_0, _1] = t;
	return std::forward_as_tuple(_0, _1);
};

template <>
constexpr inline auto get_members<3> = [](auto &t)
{
	auto &[_0, _1, _2] = t;
	return std::forward_as_tuple(_0, _1, _2);
};

template <>
constexpr inline auto get_members<4> = [](auto &t)
{
	auto &[_0, _1, _2, _3] = t;
	return std::forward_as_tuple(_0, _1, _2, _3);
};

template <>
constexpr inline auto get_members<5> = [](auto &t)
{
	auto &[_0, _1, _2, _3, _4] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4);
};

template <>
constexpr inline auto get_members<6> = [](auto &t)
{
	auto &[_0, _1, _2, _3, _4, _5] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4, _5);
};

template <>
constexpr inline auto get_members<7> = [](auto &t)
{
	auto &[_0, _1, _2, _3, _4, _5, _6] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6);
};

template <>
constexpr inline auto get_members<8> = [](auto &t)
{
	auto &[_0, _1, _2, _3, _4, _5, _6, _7] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7);
};

template <>
constexpr inline auto get_members<9> = [](auto &t)
{
	auto &[_0, _1, _2, _3, _4, _5, _6, _7, _8] = t;
	return std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7, _8);
};

template <std::size_t N>
struct StdGet {
	constexpr auto operator()(auto&& t) const noexcept
		-> decltype(std::get<N>(t))
	{
		return std::get<N>(t);
	}
};

template <class T, std::size_t N> constexpr auto get_member()
{
	static_assert(structure_binding_size_v<T> > N);
	return std::bind(StdGet<N>{},
	    std::bind(get_members<structure_binding_size_v<T>>,
	        std::placeholders::_1));
}
}

template <class T, std::size_t... Ix>
constexpr auto get_members_by_bind(std::index_sequence<Ix...>)
{
	return std::forward_as_tuple(Members::get_member<T, Ix>()...);
}

template <class T>
constexpr auto get_members(
    std::enable_if_t<std::tuple_size_v<decltype(T::members())>
                         == members_t<T>::count,
        std::nullptr_t>)
{
	return T::members();
}

template <class T>
constexpr auto get_members(
    std::enable_if_t<is_structure_bindable_v<T>> *)
{
	return get_members_by_bind<T>(
	    std::make_index_sequence<structure_binding_size_v<T>>{});
}

template <class T>
constexpr auto get_members(
    std::enable_if_t<!is_structure_bindable_v<T> &&
        members_t<T>::count == 0 && 0 < bases_t<T>::count> *)
{
	return std::tuple{};
}

namespace Name {

    template <class T>
    constexpr auto get_member_names(
        std::enable_if_t<std::tuple_size_v<decltype(T::members())>
                         == members_t<T>::count,
            std::nullptr_t>)
    {
        return std::apply([](auto ...mptr) {
            return std::tuple{
#ifndef _MSC_VER
                name<decltype(mptr), mptr>()
#else
                name<decltype(&(declval<T>().*mptr)), &(declval<T>().*mptr)>()
#endif
                ...};
        }, T::members());
    }

    template<class T>
    struct Wrapper {
        T t;
    };


    template <class T, std::size_t... Ix>
    constexpr auto get_member_names_by_bind(std::index_sequence<Ix...>)
    {
        constexpr std::tuple names = std::forward_as_tuple(Members::get_member<T, Ix>()(not_exists<T>)...);
        return names;
    }

    template <class T>
    constexpr auto get_member_names(
        std::enable_if_t<is_structure_bindable_v<T>> *)
    {
        return get_member_names_by_bind<T>(
            std::make_index_sequence<structure_binding_size_v<T>>{});
    }

    template <class T>
    constexpr auto get_member_names(
        std::enable_if_t<!is_structure_bindable_v<T> &&
                         members_t<T>::count == 0 && 0 < bases_t<T>::count> *)
    {
        return std::tuple{};
    }
}

template <class T>
struct BaseCast {
	template<class U>
	constexpr auto operator()(U& t)
		-> std::enable_if_t<std::is_base_of_v<T, U>,
	        std::conditional_t<std::is_const_v<U>, const T, T>&>
	{
		return t;
	}
};

template <class T, class... Ts> consteval auto get_bases(tuple<Ts...>)
{
	return std::tuple{BaseCast<Ts>{}...};
}

template <class T> consteval auto get_bases()
{
	return get_bases<T>(bases_t<T>{});
}

template <class T, class = void>
constexpr inline bool is_reflectable_v = false;

template <class T>
constexpr inline bool is_reflectable_v<T,
    std::void_t<decltype(get_members<T>(nullptr)),
        decltype(get_bases<T>())>> = true;


template <class T> constexpr auto getAllMembers() {
	return std::tuple_cat(get_bases<T>(), get_members<T>(nullptr));
}

struct Identity {
	template<typename _Tp>
	[[nodiscard]]
	constexpr _Tp&&
	operator()(_Tp&& __t) const noexcept
	{ return std::forward<_Tp>(__t); }
};

template <class Base, class Visitor, class T = Base
	, class Curr = decltype(std::bind(Identity{}, std::placeholders::_1))>
constexpr auto getAllFilteredMembers(Curr);

template<class Base, class Visitor, class Member,
    class M = std::remove_reference_t<std::invoke_result_t<Member, Base&>>>
constexpr auto checkMember(Member member) {
	if constexpr (std::is_invocable_v<Visitor, Member>) {
		return std::tuple{std::move(member)};
	} else {
		if constexpr (is_reflectable_v<M>) {
			return getAllFilteredMembers<Base, Visitor, M>(
			    std::move(member)
			);
		} else {
			static_assert(std::is_empty_v<M>,
			    "Class is not visitable");
			return std::tuple{};
		}
	}
}


template<class Base, class Visitor, class Curr>
struct Binder {
	constexpr auto operator()([[maybe_unused]] Curr curr, auto ... val) const  {
		return std::tuple_cat(checkMember<Base, Visitor>(std::bind(std::move(val), Curr{curr}))...);
	}
};

template <class Base, class Visitor, class T, class Curr>
constexpr auto getAllFilteredMembers(Curr curr) {
    using C = std::remove_cvref_t<Curr>;
	return std::apply([](Binder<Base, Visitor, C> b, C curr, auto...members) {
		return b(curr, members...);
	}, std::tuple_cat(std::tuple{Binder<Base, Visitor, C>{}, curr}, getAllMembers<T>()));
}

template<class T, class Visitor, class Tup = decltype(getAllFilteredMembers<T, Visitor>(std::bind(Identity{}, std::placeholders::_1))),
    class IXs = std::make_index_sequence<std::tuple_size_v<Tup>>>
struct Applier;

template<class T, class Visitor, class ...Ts, std::size_t ...Ix>
struct Applier<T, Visitor, std::tuple<Ts...>, std::index_sequence<Ix...>> {
	consteval Applier() = default;
	constexpr static inline auto members = getAllFilteredMembers<T, Visitor>(std::bind(Identity{}, std::placeholders::_1));
	constexpr auto operator()(Visitor& v) const noexcept {
        if constexpr (requires { (v( Ts{std::get<Ix>(members)}, std::initializer_list<std::string_view>{} ), ...); }) {
            [[maybe_unused]] constexpr auto names = Name::get_member_names<T>(nullptr);

            using TT = decltype((v( Ts{std::get<Ix>(members)}, std::initializer_list<std::string_view>{} ), ...));
            static_assert(!std::is_same_v<TT, TT>);
        } else {
            (v( Ts{std::get<Ix>(members)} ), ...);
        }
	}
};

template <class T, class Visitor>
constexpr auto visit(Visitor &&visitor)
{
	Applier<T, Visitor>{}(visitor);
}

template<class Visitor, class Tuple, class = std::make_index_sequence<std::tuple_size_v<Tuple>>>
struct GetterVisitor;

template<class Visitor, class ...Ts, std::size_t ...Ix>
struct GetterVisitor<Visitor, std::tuple<Ts...>, std::index_sequence<Ix...>> {
	Visitor&& visitor;
	std::tuple<Ts...> ts;

	template<class Getter>
	constexpr auto operator()(Getter&& getter) const noexcept
		-> std::invoke_result_t<Visitor&,
	        std::invoke_result_t<Getter, Ts>...> {
		return std::invoke(visitor,
		    std::invoke(getter, std::get<Ix>(ts))...);
	}
};

template <class Visitor, class T, class... Ts>
constexpr auto visit(Visitor &&visitor, T &visitable, Ts &&...ts)
    -> std::enable_if_t<(std::is_same_v<std::remove_cvref_t<T>,
                             std::remove_cvref_t<Ts>>
                         && ...)>
{
	using TT = std::remove_cvref_t<T>;
	visit<TT>(GetterVisitor<Visitor, std::tuple<T&, Ts&...>>{
	    std::forward<Visitor>(visitor),
	    {visitable, ts...}
	});
}

}

#endif