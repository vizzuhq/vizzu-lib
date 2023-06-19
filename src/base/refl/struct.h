#ifndef REFL_STRUCT
#define REFL_STRUCT

namespace Refl
{

template <typename V>
concept isVisitor =
    requires(V& visitor, int& v) { visitor(v, ""); };

template <typename S, class R>
concept isReflectable = isVisitor<R> &&
    requires(S obj, R &visitor) { obj.visit(visitor); };

}

#endif
