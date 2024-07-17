#ifndef MATH_FLOATING
#define MATH_FLOATING

#include <bit>
#include <compare>
#include <concepts>
#include <cstdint>
#include <limits>

namespace Math::Floating
{

[[nodiscard]] int orderOfMagnitude(double value, double base = 10);

constexpr auto inline less = []<std::floating_point F>(F a, F b)
{
	return std::is_lt(std::strong_order(a, b));
};

template <std::floating_point F,
    std::size_t v =
        std::numeric_limits<F>::is_iec559
                        &&std::numeric_limits<F>::radix
                    == 2
                && std::numeric_limits<unsigned char>::digits == 8
            ? sizeof(F)
            : std::size_t{}>
constexpr auto inline can_be_used_as_short_check =
    std::bool_constant<false>{};

template <std::floating_point F>
constexpr auto inline can_be_used_as_short_check<F, 4> =
    std::integral_constant<std::uint32_t,
        std::bit_cast<std::uint32_t>(F{0.0}) == std::uint32_t{}
            && std::bit_cast<std::uint32_t>(F{-0.0})
                       + std::bit_cast<std::uint32_t>(F{-0.0})
                   == std::uint32_t{}>{};

template <std::floating_point F>
constexpr auto inline can_be_used_as_short_check<F, 8> =
    std::integral_constant<std::uint64_t,
        std::bit_cast<std::uint64_t>(F{0.0}) == std::uint64_t{}
            && std::bit_cast<std::uint64_t>(F{-0.0})
                       + std::bit_cast<std::uint64_t>(F{-0.0})
                   == std::uint64_t{}>{};

constexpr auto inline is_zero = []<std::floating_point F>(F value)
{
	if constexpr ([[maybe_unused]] auto v =
	                  can_be_used_as_short_check<F>) {
		const auto val =
		    std::bit_cast<typename decltype(v)::value_type>(value);
		return val + val == 0;
	}
	else {
		return std::is_eq(std::weak_order(F{}, value));
	}
};

}

#endif
