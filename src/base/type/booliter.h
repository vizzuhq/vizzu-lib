#ifndef TYPE_BOOLITER_H
#define TYPE_BOOLITER_H

#include <array>
#include <span>

namespace Type
{
constexpr static inline std::array boolArr = {false, true};
constexpr static inline auto Bools =
    [](bool max = true) -> std::span<const bool>
{
	return {boolArr.data(), std::size_t{1} + max};
};
}

#endif
