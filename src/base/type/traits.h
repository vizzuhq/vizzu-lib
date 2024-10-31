#ifndef TYPE_TRAITS
#define TYPE_TRAITS

#include <functional>
#include <optional>
#include <type_traits>

namespace Type
{

template <typename, typename = void>
struct is_optional : std::false_type
{
	using value_type = void;
};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type
{
	using value_type = T;
};

template <typename T> concept is_optional_v = is_optional<T>::value;

template <typename T>
using optional_t = typename is_optional<T>::value_type;

template <typename, typename = void>
struct is_reference_wrapper : std::false_type
{};

template <typename T>
struct is_reference_wrapper<std::reference_wrapper<T>> :
    std::true_type
{};

template <typename T>
concept is_reference_wrapper_v = is_reference_wrapper<T>::value;

}

#endif
