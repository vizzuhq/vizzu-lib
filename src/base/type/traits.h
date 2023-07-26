#ifndef TYPE_TRAITS
#define TYPE_TRAITS

#include <istream>
#include <optional>
#include <ostream>
#include <string>

namespace Type
{

template <typename T, typename Enable = void>
struct isoptional : std::false_type
{};

template <typename T>
struct isoptional<std::optional<T>> : std::true_type
{};

}

#endif
