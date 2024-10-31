#ifndef STYLE_PARAM_H
#define STYLE_PARAM_H

#include <optional>

#include "base/type/traits.h"

namespace Style
{

template <typename T> using Param = std::optional<T>;

template <class U> concept IsParam = Type::is_optional_v<U>;

template <class T> using ParamT = Type::optional_t<T>;

}

#endif
