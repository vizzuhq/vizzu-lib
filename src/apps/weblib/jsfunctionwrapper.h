#ifndef LIB_JSFUNCTIONWRAPPER_H
#define LIB_JSFUNCTIONWRAPPER_H

#include <memory>

#include "interfacejs.h"

namespace Vizzu
{

// Unlike std::unique_ptr, the deleter of std::shared_ptr is invoked
// even if the managed pointer is null -> keep unique_ptr
template <class R, class... Ts>
using JsFunctionWrapper =
    std::unique_ptr<R(std::remove_reference_t<Ts> *...),
        void (*)(R(std::remove_reference_t<Ts> *...))>;

}

#endif
