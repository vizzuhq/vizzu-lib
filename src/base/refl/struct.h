#ifndef REFL_STRUCT
#define REFL_STRUCT

namespace Refl
{

template <typename V, typename O>
concept isVisitor =
    requires(V visitor, O obj) { visitor(obj, ""); };

template <typename S, class R>
concept isReflectable = isVisitor<R, S> &&
    requires(S obj, R &visitor) { obj.visit(visitor); };

}

#endif
