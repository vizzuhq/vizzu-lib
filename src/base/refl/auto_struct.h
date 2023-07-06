#ifndef VIZZU_REFL_AUTO_STRUCT_H
#define VIZZU_REFL_AUTO_STRUCT_H

#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace Refl
{

template <class... Ts> struct tuple
{
	template <std::size_t I>
	using type = std::tuple_element_t<I, std::tuple<Ts...>>;

	constexpr static inline auto count = sizeof...(Ts);
};

namespace Size
{
struct ubiq
{
	std::size_t ignore;
	template <class Type> constexpr operator Type &() const noexcept
	{
		return std::declval<Type &>();
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
		return std::declval<Type &>();
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
    typename std::add_pointer<decltype(T{ubiq_base<T>{B}...,
        ubiq{I}...})>::type;

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
template <class T, std::size_t N> struct tag
{
	friend auto loophole(tag<T, N>);
};

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
		return std::declval<U &&>();
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
constexpr inline auto std_get = [](auto &&t) -> decltype(auto)
{
	return std::get<N>(t);
};

template <class T, std::size_t N> consteval auto get_member()
{
	static_assert(structure_binding_size_v<T> > N);
	return std::bind(std_get<N>,
	    std::bind(get_members<structure_binding_size_v<T>>,
	        std::placeholders::_1));
}
}

template <class T, std::size_t... Ix>
consteval auto get_members_by_bind(std::index_sequence<Ix...>)
{
	return std::forward_as_tuple(Members::get_member<T, Ix>()...);
}

template <class T>
consteval auto get_members(
    std::enable_if_t<std::tuple_size_v<decltype(T::members())>
                         == members_t<T>::count,
        std::nullptr_t>)
{
	return T::members();
}

template <class T>
consteval auto get_members(
    std::enable_if_t<is_structure_bindable_v<T>> *)
{
	return get_members_by_bind<T>(
	    std::make_index_sequence<structure_binding_size_v<T>>{});
}

template <class T>
constexpr inline auto base_cast =
    []<class U>(
        U &t) -> std::conditional_t<std::is_const_v<U>, const T, T> &
{
	return t;
};

template <class T, class... Ts> consteval auto get_bases(tuple<Ts...>)
{
	return std::tuple{base_cast<Ts>...};
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

namespace Name
{
template <auto> consteval auto get_member_name()
{
#ifdef _MSC_VER
	constexpr std::string_view sv = __FUNCSIG__;
	constexpr auto last = sv.find_last_not_of(" }>)(", sv.size() - 6);
#else
	constexpr std::string_view sv = __PRETTY_FUNCTION__;
	constexpr auto last = sv.find_last_not_of(" }])");
#endif
	constexpr auto first =
	    sv.find_last_not_of("abcdefghijklmnopqrstuvwxyz"
	                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	                        "_0123456789",
	        last);
	std::array<char, last - first + 1> res{};
	auto it = res.data();
	for (auto a = first + 1; a <= last; ++a) *it++ = sv[a];
	return res;
}

}

template <class T, class Visitor> consteval auto getMemberPointers()
{
	return [](Visitor &)
	{

	};
}

template <class T, class Visitor>
constexpr auto visit(Visitor &&visitor)
{
	getMemberPointers<T, Visitor>()(visitor);

	// invocable ->

	// reflectable ->

	// assert, empty
	/*
	        using TV = std::remove_cvref_t<T>;
	        if constexpr (std::is_invocable_v<Visitor, T&>) {
	            visitor(visitable);
	        } else if constexpr (is_reflectable_v<TV>) {

	            std::apply(
	                [&visitor](auto &...args)
	                {
	                    (visit(visitor, args), ...);
	                },
	                std::tuple_cat(get_bases<TV>()(visitable),
	                    get_members<TV>(nullptr)(visitable)));

	        }
	        else {
	            static_assert(std::is_empty_v<T>, "Class is not
	   visitable");
	        }
	        */
}

template <class Visitor, class T, class... Ts>
constexpr auto visit(Visitor &&visitor, T &visitable, Ts &&...ts)
    -> std::enable_if_t<(std::is_same_v<std::remove_cvref_t<T>,
                             std::remove_cvref_t<Ts>>
                         && ...)>
{
	using TT = std::remove_cvref_t<T>;
	visit<TT>(
	    [&]<class Getter>(
	        Getter &&getter) -> std::invoke_result_t<Visitor,
	                             std::invoke_result_t<Getter, T>,
	                             std::invoke_result_t<Getter, Ts>...>
	    {
		    return std::invoke(visitor,
		        std::invoke(getter, visitable),
		        std::invoke(getter, ts...));
	    });
}

}

#endif