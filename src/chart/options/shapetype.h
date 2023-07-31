#ifndef SHAPETYPE_H
#define SHAPETYPE_H

#include <array>
#include <string>

#include "base/math/fuzzybool.h"

namespace Vizzu::Gen
{

enum class ShapeType : uint32_t { rectangle, circle, area, line };

bool isConnecting(ShapeType type);

}

#endif
