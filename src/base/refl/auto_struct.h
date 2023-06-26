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

template <class T, class> struct separate;

template <class T, class... Elements>
struct separate<T, tuple<Elements...>>
{
	using Tup = tuple<Elements...>;
	consteval static std::size_t get_base_count()
	{
		std::size_t ix{};
		for (bool b : std::initializer_list<bool>{
		         std::is_base_of_v<Elements, T>...})
			if (b)
				++ix;
			else
				break;

		return ix;
	}

	template <std::size_t... Ix>
	consteval static auto get_base_types(std::index_sequence<Ix...>)
	    -> tuple<typename Tup::template type<Ix>...>;

	template <std::size_t N, std::size_t... Ix>
	consteval static auto get_member_types(std::index_sequence<Ix...>)
	    -> tuple<typename Tup::template type<N + Ix>...>;

	using bases = decltype(get_base_types(
	    std::make_index_sequence<get_base_count()>{}));
	using members = decltype(get_member_types<get_base_count()>(
	    std::make_index_sequence<sizeof...(Elements)
	                             - get_base_count()>{}));
};

namespace Size
{
struct ubiq
{
	std::size_t ignore;
	template <class U> consteval operator U &() const && noexcept;
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

template <class T,
    std::size_t N,
    class = decltype(enable_if_constructible<T>(
        std::make_index_sequence<N>()))>
using enable_if_constructible_t = std::size_t;

template <class T, std::size_t Begin, std::size_t Middle>
consteval std::size_t detect_fields_count(std::true_type,
    int) noexcept
{
	return Begin;
}

template <class T, std::size_t Begin, std::size_t Middle>
consteval std::size_t detect_fields_count(std::false_type,
    long) noexcept;

template <class T, std::size_t Begin, std::size_t Middle>
consteval auto detect_fields_count(std::false_type, int) noexcept
    -> enable_if_constructible_t<T, Middle>
{
	constexpr std::size_t next_v = Middle + (Middle - Begin + 1) / 2;
	return detect_fields_count<T, Middle, next_v>(
	    std::bool_constant<Middle == next_v>{},
	    0);
}

template <class T, std::size_t Begin, std::size_t Middle>
consteval std::size_t detect_fields_count(std::false_type,
    long) noexcept
{
	constexpr std::size_t next_v = Begin + (Middle - Begin) / 2;
	return detect_fields_count<T, Begin, next_v>(
	    std::bool_constant<Begin == next_v>{},
	    0);
}

template <class T> consteval std::size_t aggregate_count() noexcept
{
	using type = std::remove_cvref_t<T>;
	if constexpr (std::is_empty_v<type> || std::is_polymorphic_v<type>
	              || !std::is_aggregate_v<type>
	              || std::is_scalar_v<type>) {
		return 0;
	}
	else {
		return detect_fields_count<T,
		    0,
		    sizeof(type)
		        * std::numeric_limits<unsigned char>::digits>(
		    std::bool_constant<false>{},
		    0);
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

template <class T, std::size_t N> struct loophole_ubiq
{
	template <class U, std::size_t M> static std::size_t ins(...);
	template <class U,
	    std::size_t M,
	    std::size_t = sizeof(loophole(tag<T, M>{}))>
	static char ins(int);

	template <class U,
	    std::size_t = sizeof(
	        fn_def<T, U, N, sizeof(ins<U, N>(0)) == sizeof(char)>)>
	consteval operator U &() const && noexcept;
};

template <class T,
    class U = std::make_index_sequence<Size::aggregate_count<T>()>>
struct loophole_type_list;

template <typename T, std::size_t... I>
struct loophole_type_list<T, std::index_sequence<I...>> :
    Refl::tuple<decltype(T{loophole_ubiq<T, I>{}...}, 0)>
{
	using type = Refl::tuple<
	    std::remove_reference_t<decltype(*loophole(tag<T, I>{}))>...>;
};

template <class T>
using aggregate_types_t =
    typename Loophole::loophole_type_list<T>::type;
}

template <class T>
using bases_t =
    typename separate<T, Loophole::aggregate_types_t<T>>::bases;

template <class T>
using members_t =
    typename separate<T, Loophole::aggregate_types_t<T>>::members;

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

template <class T, class Bases = bases_t<T>>
constexpr inline std::size_t structure_binding_size_v{};

template <class T, class... Base>
constexpr inline std::size_t
    structure_binding_size_v<T, tuple<Base...>> = []
{
	if constexpr (members_t<T>::count > 0) {
		return members_t<T>::count;
	}
	else {
		return ((is_structure_bindable_v<Base>
		            * structure_binding_size_v<Base>)+...
		        + 0);
	}
}();

namespace Members
{
template <std::size_t I>
using count_t = std::integral_constant<std::size_t, I>;

template <class T, count_t<0>>
constexpr auto get_members(T &t) = delete;

template <class T, count_t<1>> constexpr auto get_members(T &t)
{
	auto &[_0] = t;
	return std::forward_as_tuple(_0);
}

template <class T, count_t<2>> constexpr auto get_members(T &t)
{
	auto &[_0, _1] = t;
	return std::forward_as_tuple(_0, _1);
}

template <class T, count_t<3>> constexpr auto get_members(T &t)
{
	auto &[_0, _1, _2] = t;
	return std::forward_as_tuple(_0, _1, _2);
}
}

template <class T>
consteval auto get_members(
    std::enable_if_t<is_structure_bindable_v<T>, std::nullptr_t>)
{
	return &Members::get_members<T,
	    Members::count_t<structure_binding_size_v<T>>{}>;
}

template <class T, auto... ptrs>
consteval auto get_members_by_pointers()
{
	return +[](T &t)
	{
		return std::forward_as_tuple(t.*ptrs...);
	};
}
template <class T, std::size_t... Ix>
consteval auto get_members_by_pointer_list(std::index_sequence<Ix...>)
{
	return get_members_by_pointers<T,
	    std::get<Ix>(T::members())...>();
}

template <class T>
consteval auto get_members(
    std::enable_if_t<std::tuple_size_v<decltype(T::members())>
                     == members_t<T>::count> *)
{
	return get_members_by_pointer_list<T>(std::make_index_sequence<
	    std::tuple_size_v<decltype(T::members())>>{});
}

template <class T, class = void>
constexpr inline bool is_reflectable_v = is_structure_bindable_v<T>;

template <class T>
constexpr inline bool
    is_reflectable_v<T, std::void_t<decltype(T::members())>> = true;

template <class T, class... Ts> consteval auto get_bases(tuple<Ts...>)
{
	return +[](T &obj)
	{
		return std::forward_as_tuple(static_cast<
		    std::conditional_t<std::is_const_v<T>, const Ts, Ts> &>(
		    obj)...);
	};
}
template <class T> consteval auto get_bases()
{
	return get_bases<T>(bases_t<T>{});
}

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

template <class T> constexpr void visit(auto &visitor, T &visitable)
{
	if constexpr (is_reflectable_v<T>) {
		std::apply(
		    [&visitor](auto &...args)
		    {
			    (visit(visitor, args), ...);
		    },
		    std::tuple_cat(get_bases<T>()(visitable),
		        get_members<T>(nullptr)(visitable)));
	}
	else if constexpr (!std::is_empty_v<T>) {
		visitor(visitable);
	}
}

}

#endif