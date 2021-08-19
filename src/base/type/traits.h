#ifndef TYPE_TRAITS
#define TYPE_TRAITS

#include <istream>
#include <ostream>
#include <string>
#include <optional>

namespace Type
{

template<typename T>
concept istreamable = requires(std::istream &stream, T p) { stream >> p; };

template<typename T>
concept ostreamable = requires(std::ostream &stream, T p) { stream << p; };

template <typename T, typename Enable = void>
struct isoptional : std::false_type
{};

template <typename T>
struct isoptional<std::optional<T>> : std::true_type
{};

}

#endif
