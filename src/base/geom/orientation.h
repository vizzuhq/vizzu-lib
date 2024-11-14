#ifndef BASE_GEOM_ORIENTATION_H
#define BASE_GEOM_ORIENTATION_H

#include <cstdint>

namespace Geom
{

enum class Orientation : bool { horizontal, vertical };

constexpr bool isHorizontal(Orientation orientation)
{
	return orientation == Orientation::horizontal;
}

constexpr Orientation operator!(Orientation orientation)
{
	return isHorizontal(orientation) ? Orientation::vertical
	                                 : Orientation::horizontal;
}

}

#endif
