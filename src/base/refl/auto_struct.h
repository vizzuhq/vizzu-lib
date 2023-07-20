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

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

template <class T> extern T not_exists;

template <class T> consteval T declval()
{
	return std::forward<T>(not_exists<std::remove_reference_t<T>>);
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

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
    std::tuple<decltype(T{loophole_ubiq<T, B, true>{}...,
                            loophole_ubiq<T, sizeof...(B) + I>{}...},
        0)>
{
	using type = std::tuple<std::tuple<std::remove_reference_t<
	                            decltype(*loophole(tag<T, B>{}))>...>,
	    std::tuple<std::remove_reference_t<decltype(*loophole(
	        tag<T, sizeof...(B) + I>{}))>...>>;
};

template <class T>
using aggregate_types_t =
    typename Loophole::loophole_type_list<T>::type;
}

template <class T>
using bases_t =
    std::tuple_element_t<0, Loophole::aggregate_types_t<T>>;

template <class T>
using members_t =
    std::tuple_element_t<1, Loophole::aggregate_types_t<T>>;

template <class T, std::size_t = std::tuple_size_v<members_t<T>>>
constexpr inline bool not_same_as_decomposed = true;

template <class T>
constexpr inline bool not_same_as_decomposed<T, 1> = !std::is_same_v<
    std::remove_cv_t<std::tuple_element_t<0, members_t<T>>>,
    std::remove_cv_t<T>>;

template <class T, class Bases = bases_t<T>>
constexpr inline bool is_structure_bindable_v = false;

template <class T, class... Base>
constexpr inline bool
    is_structure_bindable_v<T, std::tuple<Base...>> =
        (not_same_as_decomposed<T>
            && std::tuple_size_v<Loophole::aggregate_types_t<T>> > 0)
        && (((is_structure_bindable_v<Base> + ... + 0) == 1
                && std::tuple_size_v<members_t<T>> == 0)
            || (std::is_empty_v<Base> && ... && !std::is_empty_v<T>));

template <class T,
    bool = is_structure_bindable_v<T>,
    class Bases = bases_t<T>>
constexpr inline std::size_t structure_binding_size_v{};

template <class T, class... Base>
constexpr inline std::size_t
    structure_binding_size_v<T, true, std::tuple<Base...>> =
        (structure_binding_size_v<Base>
            + ... + std::tuple_size_v<members_t<T>>);

template <auto... ils> struct Refs
{};

namespace CompositeOp
{
template <class T, class U>
constexpr inline auto operator>>(U &&arg, const T &op)
    -> decltype(op(std::forward<U &&>(arg)))
{
	return op(std::forward<U &&>(arg));
}

template <class, class...> struct CompositeImpl;

template <std::size_t... Ix, class... Ts>
struct CompositeImpl<std::index_sequence<Ix...>, Ts...>
{
	consteval CompositeImpl() = default;
	template <class T>
	constexpr inline decltype(auto) operator()(T &&val) const noexcept
	{
		using namespace CompositeOp;
		return (std::forward<T &&>(val) >> ...
		        >> std::tuple_element_t<Ix, std::tuple<Ts...>>{});
	}

	consteval static auto get_names()
	{
		return []<std::size_t... Jx>(std::index_sequence<Jx...>)
		{
			static_assert(sizeof...(Jx) > 0);
			constexpr auto L = [](auto asd)
			{
				if constexpr (requires {
					              decltype(asd)::type::getName();
				              }) {
					return std::tuple{decltype(asd)::type::getName()};
				}
				else {
					return std::tuple{};
				}
			};
			constexpr auto names =
			    std::tuple_cat(L(std::type_identity<Ts>{})...);

			return Refs<std::get<Jx>(names)...>{};
		}(std::make_index_sequence<(
		        (requires { Ts::getName(); }) + ...)>{});
	};
};
}

template <class T, class U> struct CompositeRec
{
	using type =
	    CompositeOp::CompositeImpl<std::make_index_sequence<2>, T, U>;
};
template <class A, class... Ts, class U>
struct CompositeRec<U, CompositeOp::CompositeImpl<A, Ts...>>
{
	using type = CompositeOp::CompositeImpl<
	    std::make_index_sequence<sizeof...(Ts) + 1>,
	    U,
	    Ts...>;
};

template <class A, class... Ts, class U>
struct CompositeRec<CompositeOp::CompositeImpl<A, Ts...>, U>
{
	using type = CompositeOp::CompositeImpl<
	    std::make_index_sequence<sizeof...(Ts) + 1>,
	    Ts...,
	    U>;
};

template <class A, class... Ts, class B, class... Us>
struct CompositeRec<CompositeOp::CompositeImpl<B, Us...>,
    CompositeOp::CompositeImpl<A, Ts...>>
{
	using type = CompositeOp::CompositeImpl<
	    std::make_index_sequence<sizeof...(Us) + sizeof...(Ts)>,
	    Us...,
	    Ts...>;
};

template <class T, class U>
using Composite = typename CompositeRec<T, U>::type;

namespace Members
{
template <std::size_t> struct get_members
{};

template <> struct get_members<1>
{
	template <class U>
	constexpr inline auto operator()(U &&t) const noexcept
	{
		auto &[_0] = t;
		return std::forward_as_tuple(_0);
	}
};

template <> struct get_members<2>
{
	template <class U>
	constexpr inline auto operator()(U &&t) const noexcept
	{
		auto &[_0, _1] = t;
		return std::forward_as_tuple(_0, _1);
	}
};

template <> struct get_members<3>
{
	template <class U>
	constexpr inline auto operator()(U &&t) const noexcept
	{
		auto &[_0, _1, _2] = t;
		return std::forward_as_tuple(_0, _1, _2);
	}
};

template <> struct get_members<4>
{
	template <class U>
	constexpr inline auto operator()(U &&t) const noexcept
	{
		auto &[_0, _1, _2, _3] = t;
		return std::forward_as_tuple(_0, _1, _2, _3);
	}
};

template <> struct get_members<5>
{
	template <class U>
	constexpr inline auto operator()(U &&t) const noexcept
	{
		auto &[_0, _1, _2, _3, _4] = t;
		return std::forward_as_tuple(_0, _1, _2, _3, _4);
	}
};

template <> struct get_members<6>
{
	template <class U>
	constexpr inline auto operator()(U &&t) const noexcept
	{
		auto &[_0, _1, _2, _3, _4, _5] = t;
		return std::forward_as_tuple(_0, _1, _2, _3, _4, _5);
	}
};

template <> struct get_members<7>
{
	template <class U>
	constexpr inline auto operator()(U &&t) const noexcept
	{
		auto &[_0, _1, _2, _3, _4, _5, _6] = t;
		return std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6);
	}
};

template <> struct get_members<8>
{
	template <class U>
	constexpr inline auto operator()(U &&t) const noexcept
	{
		auto &[_0, _1, _2, _3, _4, _5, _6, _7] = t;
		return std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7);
	}
};

template <> struct get_members<9>
{
	template <class U>
	constexpr inline auto operator()(U &&t) const noexcept
	{
		auto &[_0, _1, _2, _3, _4, _5, _6, _7, _8] = t;
		return std::forward_as_tuple(_0,
		    _1,
		    _2,
		    _3,
		    _4,
		    _5,
		    _6,
		    _7,
		    _8);
	}
};

template <std::size_t N> struct StdGet
{
	constexpr inline auto operator()(auto &&t) const noexcept
	    -> decltype(std::get<N>(t))
	{
		return std::get<N>(t);
	}
};

template <class T> struct Wrapper
{
	T t;
};

template <class T> Wrapper(T) -> Wrapper<T>;

template <class T, std::size_t N>
struct CompositeMember : std::identity
{

	using Base = Composite<get_members<structure_binding_size_v<T>>,
	    StdGet<N>>;
	consteval static auto getName()
	{
		return Refl::Name::name<void,
		    Wrapper{&std::invoke(Base{}, declval<T &>())}>();
	}
};

template <class T, std::size_t N> consteval auto get_member()
{
	static_assert(structure_binding_size_v<T> > N);
	return Composite<CompositeMember<T, N>,
	    Composite<get_members<structure_binding_size_v<T>>,
	        StdGet<N>>>{};
}
}

template <class T, std::size_t... Ix>
consteval auto get_members_by_bind(std::index_sequence<Ix...>)
{
	return std::tuple{Members::get_member<T, Ix>()...};
}

template <auto P> struct MemberCast
{
	template <class Base, class Member>
	static Base getBase(Member Base::*);

	consteval static auto getName()
	{
#if !defined(__clang__) && defined(__GNUC__)
		return Refl::Name::name<decltype(P), P>();
#else
		return Refl::Name::name<void,
		    Members::Wrapper{&std::invoke(P,
		        declval<decltype(getBase(P)) &>())}>();
#endif
	}

	template <class U>
	constexpr inline auto operator()(U &t) const noexcept
	    -> std::invoke_result_t<decltype(P), U &>
	{
		return std::invoke(P, t);
	}
};

template <auto ptr,
    std::enable_if_t<
        !std::is_same_v<std::remove_cvref_t<decltype(ptr)>,
            std::remove_cvref_t<decltype(std::ignore)>>> * = nullptr>
consteval auto get_mem_fn()
{
	return std::tuple{MemberCast<ptr>{}};
}

template <decltype(std::ignore)> consteval auto get_mem_fn()
{
	return std::tuple{};
}

template <class T, std::size_t... Ix>
consteval auto get_members_by_memptrs(std::index_sequence<Ix...>)
{
	return std::tuple_cat(
	    get_mem_fn<std::get<Ix>(T::members())>()...);
}

template <class T>
consteval auto get_members(
    std::enable_if_t<std::tuple_size_v<decltype(T::members())> != 0
                         && std::tuple_size_v<members_t<T>> != 0,
        std::nullptr_t>)
{
	static_assert(std::tuple_size_v<decltype(T::members())>
	                  == std::tuple_size_v<members_t<T>>,
	    "Not specified all member");
	return get_members_by_memptrs<T>(
	    std::make_index_sequence<std::tuple_size_v<members_t<T>>>{});
}

template <class T>
consteval auto get_members(
    std::enable_if_t<is_structure_bindable_v<T>> *)
{
	return get_members_by_bind<T>(
	    std::make_index_sequence<structure_binding_size_v<T>>{});
}

template <class T>
consteval auto get_members(
    std::enable_if_t<!is_structure_bindable_v<T>
                     && std::tuple_size_v<members_t<T>> == 0
                     && 0 < std::tuple_size_v<bases_t<T>>> *)
{
	return std::tuple{};
}

template <class T> struct BaseCast
{
	template <class U>
	constexpr inline auto operator()(U &t) const noexcept
	    -> std::enable_if_t<std::is_base_of_v<T, U>,
	        std::conditional_t<std::is_const_v<U>, const T, T> &>
	{
		return t;
	}
};

template <class T, class... Ts>
consteval auto get_bases(std::tuple<Ts...> *)
{
	return std::tuple{BaseCast<Ts>{}...};
}

template <class T> consteval auto get_bases()
{
	return get_bases<T>(std::add_pointer_t<bases_t<T>>{});
}

template <class T, class = void>
constexpr inline bool is_reflectable_v = false;

template <class T>
constexpr inline bool is_reflectable_v<T,
    std::void_t<decltype(get_members<T>(nullptr)),
        decltype(get_bases<T>())>> = true;

template <class T> consteval auto getAllMembers()
{
	return std::tuple_cat(get_bases<T>(), get_members<T>(nullptr));
}

template <class Base,
    class Visitor,
    class T = Base,
    class Curr = std::identity>
consteval auto getAllFilteredMembers(Curr = {});

template <class Base,
    class Visitor,
    class Member,
    class M =
        std::remove_reference_t<std::invoke_result_t<Member, Base &>>>
consteval auto checkMember(Member member)
{
	if constexpr (std::is_invocable_v<Visitor,
	                  M &(*)(const Base &)>) {
		return std::tuple{std::move(member)};
	}
	else {
		if constexpr (is_reflectable_v<M>) {
			return getAllFilteredMembers<Base, Visitor, M>(
			    std::move(member));
		}
		else {
			static_assert(std::is_empty_v<M>,
			    "Class is not visitable");
			return std::tuple{};
		}
	}
}

template <class Base, class Visitor, class Curr> struct Binder
{
	consteval auto operator()(auto... val) const
	{
		return std::tuple_cat(checkMember<Base, Visitor>(
		    Composite<Curr, decltype(val)>{})...);
	}
};

template <class Base, class Visitor, class T, class Curr>
consteval auto getAllFilteredMembers(Curr)
{
	return []<std::size_t... Ix>(auto tup, std::index_sequence<Ix...>)
	{
		return Binder<Base, Visitor, std::remove_cvref_t<Curr>>{}(
		    std::get<Ix>(tup)...);
	}(getAllMembers<T>(),
	    std::make_index_sequence<
	        std::tuple_size_v<decltype(getAllMembers<T>())>>{});
}

namespace Name
{

template <class T, class Visitor>
consteval inline auto get_member_names()
{
	return []<std::size_t... Ix>(std::index_sequence<Ix...>)
	{
		constexpr auto M = getAllFilteredMembers<T, Visitor>();
		return std::tuple(
		    std::remove_reference_t<decltype(std::get<Ix>(
		        M))>::get_names()...);
	}(std::make_index_sequence<std::tuple_size_v<
	        decltype(getAllFilteredMembers<T, Visitor>())>>{});
}
}

template <class T,
    class Visitor,
    class IXs = std::make_index_sequence<std::tuple_size_v<
        decltype(getAllFilteredMembers<T, Visitor>())>>>
struct Applier;

template <class T, class V>
constexpr static inline auto fptr = +[](const T &v) -> auto &
{
	auto &m = V{}(v);
	return const_cast<std::remove_cvref_t<decltype(m)> &>(m);
};

template <auto A>
constexpr static inline std::string_view NData{std::data(A),
    std::size(A)};

template <class arg> constexpr static inline std::nullptr_t Ldata{};

template <auto... args>
constexpr static inline std::initializer_list<std::string_view>
    Ldata<Refs<args...>>{NData<args>...};

template <class T, class Visitor, std::size_t... Ix>
struct Applier<T, Visitor, std::index_sequence<Ix...>>
{
	consteval Applier() = default;

	constexpr inline __attribute__((always_inline)) void operator()(
	    Visitor &v) const noexcept
	{
		constexpr auto members = getAllFilteredMembers<T, Visitor>();
		static_assert(std::tuple_size_v<decltype(members)> > 0);
		if constexpr ((std::is_invocable_v<Visitor &,
		                   decltype(fptr<T,
		                       decltype(std::get<Ix>(members))>),
		                   std::initializer_list<std::string_view>>
		                  && ...)) {
			[[maybe_unused]] constexpr auto names =
			    Name::get_member_names<T, Visitor>();
			static_assert(std::tuple_size_v<decltype(names)>
			              == std::tuple_size_v<decltype(members)>);
			(v(fptr<T, decltype(std::get<Ix>(members))>,
			     Ldata<std::remove_cvref_t<decltype(std::get<Ix>(
			         names))>>),
			    ...);
		}
		else
			(v(fptr<T, decltype(std::get<Ix>(members))>), ...);
	}
};

template <class T, class Visitor>
constexpr inline __attribute__((always_inline)) void visit(
    Visitor &&visitor)
{
	Applier<T, Visitor>{}(visitor);
}

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

	constexpr inline GetterVisitor(Visitor &&visitor,
	    std::tuple<Ts...> t) :
	    visitor(std::forward<Visitor>(visitor)),
	    ts(std::move(t))
	{}

	template <class Getter>
	constexpr inline auto operator()(Getter &&getter) const noexcept
	    -> std::invoke_result_t<Visitor &,
	        std::invoke_result_t<Getter, Ts>...>
	{
		return std::invoke(visitor,
		    std::invoke(getter, std::get<Ix>(ts))...);
	}
};

template <class Visitor, class T, class... Ts>
constexpr inline auto
visit(Visitor &&visitor, T &visitable, Ts &&...ts)
    -> std::enable_if_t<(std::is_same_v<std::remove_cvref_t<T>,
                             std::remove_cvref_t<Ts>>
                         && ...)>
{
	using TT = std::remove_cvref_t<T>;
	visit<TT>(GetterVisitor<Visitor, std::tuple<T &, Ts &...>>{
	    std::forward<Visitor>(visitor),
	    {visitable, ts...}});
}

}

#endif