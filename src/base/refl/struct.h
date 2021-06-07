#ifndef REFL_STRUCT
#define REFL_STRUCT

namespace Refl
{

template <typename S, class R>
concept isReflectable = requires(S obj, R &visitor)
{
	obj.visit(visitor);
};

}

#endif
