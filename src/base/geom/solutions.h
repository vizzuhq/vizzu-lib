#ifndef GEOM_SOLUTIONS
#define GEOM_SOLUTIONS

#include <array>
#include <optional>

namespace Geom
{

template <typename T, unsigned N>
using Solutions = std::array<std::optional<T>, N>;

}

#endif
